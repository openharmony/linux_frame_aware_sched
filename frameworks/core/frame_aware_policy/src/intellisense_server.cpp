/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "intellisense_server.h"
#include <fcntl.h>
#include <hitrace_meter.h>
#include <list>
#include <map>
#include <new>
#include <set>
#include <securec.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include "qos_common.h"
#include "para_config.h"
#include "rtg_interface.h"
#include "rme_log_domain.h"

#undef LOG_TAG
#define LOG_TAG "ueaServer-IntelliSenseServer"

namespace OHOS {
namespace RME {
namespace {
    static std::string configFilePath = "/system/etc/frame_aware_sched/hwrme.xml"; // need To check the exact file path.
    constexpr int WEB_BASE_UID = 1000001;
    constexpr int WEB_END_UID = 1099999;
    const char RTG_SCHED_IPC_MAGIC = 0xAB;
    constexpr int RTG_TYPE_MAX = 3;
}
using namespace std;
using namespace QosCommon;

IMPLEMENT_SINGLE_INSTANCE(IntelliSenseServer);

#define CMD_ID_SET_RTG \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_RTG, struct rtg_str_data)

void IntelliSenseServer::Init()
{
    if (!ReadXml()) {
        RME_LOGI("[Init]: readXml failed!");
        return;
    }
    m_switch = std::stoi(m_generalPara["enable"]);
    if (!m_switch) {
        RME_LOGI("[Init]:xml switch close!");
        return;
    }
    int ret = EnableRtg(true);
    if (ret < 0) {
        RME_LOGE("[Init]: enable rtg failed!");
        return;
    }
    m_unsupportApp = {
        "com.ohos.launcher",
        "com.ohos.systemui",
        "com.ohos.screenlock",
        "com.ohos.wallpaper"
    };
    RME_LOGI("[Init]:Init rtg and readXml finish!");
}

bool IntelliSenseServer::ReadXml()
{
    if (!m_needReadXml) {
        return false;
    }
    m_needReadXml = false;
    if (ParaConfig::IsXmlPrepared(configFilePath)) {
        m_generalPara = ParaConfig::GetGeneralConfig();
        m_subEventPara = ParaConfig::GetSubEventConfig();
        m_fpsList = ParaConfig::GetFpsList();
        m_renderTypeList = ParaConfig::GetRenderTypeList();
        if (!m_generalPara.empty() && !m_subEventPara.empty() && !m_fpsList.empty() && !m_renderTypeList.empty()) {
            m_readXmlSuc = true;
            RME_LOGI("[ReadXml]: read slide scene xml success!");
            return true;
        }
    }
    RME_LOGE("[ReadXml]: read slide scene xml not success!");
    return false;
}

void IntelliSenseServer::NewForeground(int pid, int uid)
{
    HITRACE_METER(HITRACE_TAG_ACE);
    int newCreatedRtg = 0;
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppUid() == uid) {
            RME_LOGI("[ReportMessage]pid %{public}d change to foreground.", pid);
            if (iter->GetAppState() != AppState::APP_FOREGROUND) {
                iter->SetUiTid(pid);
                newCreatedRtg = TryCreateRtgForApp(&*iter);
            }
            if (newCreatedRtg) {
                iter->SetAppState(AppState::APP_FOREGROUND);
            } else {
                iter->SetAppState(AppState::APP_FOREGROUND_WITHOUT_RTG);
            }
            break;
        }
    }
}

int IntelliSenseServer::TryCreateRtgForApp(AppInfo *app)
{
    if (!app) {
        RME_LOGE("[TryCreateRtg]: null app!");
        return 0;
    }
    int grpId = CreateNewRtgGrp(RT_PRIO, RT_NUM);
    if (grpId <= 0) {
        RME_LOGE("[TryCreateRtg]: createNewRtgGroup failed! grpId:%{public}d", grpId);
        app->SetRtgrp(0);
        return grpId;
    }
    app->SetRtgrp(grpId);
    int uiTid = app->GetUiTid();
    int renderTid = app->GetRenderTid();
    if (uiTid > 0) {
        AddThreadToRtg(uiTid, grpId, 0); // add ui thread
    }
    if (renderTid > 0) {
        AddThreadToRtg(renderTid, grpId, 0); // add render thread
    }
    return grpId;
}

void IntelliSenseServer::NewBackground(int pid)
{
    HITRACE_METER(HITRACE_TAG_ACE);
    RME_LOGI("[ReportMessage]pid %{public}d change to background.", pid);
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppPid() != pid) {
            continue;
        }
        iter->SetAppState(AppState::APP_BACKGROUND);
        int grpId = iter->GetRtgrp();
        if (grpId > 0) {
            EndScene(grpId);
            DestroyRtgGrp(grpId);
        }
    }
}

void IntelliSenseServer::NewAppRecord(int pid, int uid)
{
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppUid() == uid) {
            RME_LOGI("[NewAppRecord]pid %{public}d already exist.", pid);
            return;
        }
    }
    AppInfo tempRecord(pid, uid);
    tempRecord.SetAppState(AppState::APP_FOREGROUND_WITHOUT_RTG);
    m_historyApp.push_back(tempRecord);
}

void IntelliSenseServer::NewDiedProcess(int pid)
{
    HITRACE_METER(HITRACE_TAG_ACE);
    RME_LOGI("[ReportMessage]pid %{public}d died.", pid);
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end();) {
        if (iter->GetAppPid() == pid) {
            int grpId = iter->GetRtgrp();
            if (grpId > 0) {
                EndScene(grpId);
                DestroyRtgGrp(grpId);
            }
            iter = m_historyApp.erase(iter);
        } else {
            iter++;
        }
    }
}

std::list<AppInfo>::iterator IntelliSenseServer::GetRecordOfPid(int pid)
{
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppPid() == pid) {
            return iter;
        }
    }
    return m_historyApp.end();
}

void IntelliSenseServer::ReportRenderThread(const int pid, const int uid, int renderTid)
{
    if (uid >= WEB_BASE_UID && uid <= WEB_END_UID) {
        return;
    }
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    auto record = GetRecordOfPid(pid);
    if (record == m_historyApp.end()) {
        RME_LOGE("Didn't find render in history app %{public}d with render %{public}d", pid, renderTid);
        return;
    }
    record->SetRenderTid(renderTid);
    int grpId = record->GetRtgrp();
    if (grpId >= 0 && record->GetAppState() == AppState::APP_FOREGROUND) {
        int ret = AddThreadToRtg(renderTid, grpId, 0); // add render thread
        RME_LOGE("web test ReportRenderThread uid is %{public}d", uid);
        if (ret != 0) {
            RME_LOGE("[OnFore]:add render thread fail! pid:%{public}d,rtg:%{public}d!ret:%{publid}d",
                renderTid, grpId, ret);
        }
    }
}

void IntelliSenseServer::ReportContinuousTask(const int pid, const int uid, const int status)
{
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    switch (status) {
        case static_cast<int>(ContinuousTaskState::TASK_START):
            RME_LOGI("[ReportContinuousTask]:%{public}d continuous task start", pid);
            break;
        case static_cast<int>(ContinuousTaskState::TASK_END):
            RME_LOGI("[ReportContinuousTask]:%{public}d continuous task end", pid);
            break;
        default:
            RME_LOGI("[ReportContinuousTask]:unknown continuous task status!");
            break;
    }
}

void IntelliSenseServer::ReportWindowFocus(const int pid, const int uid, int isFocus)
{
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    switch (isFocus) {
        case static_cast<int>(WindowState::FOCUS_YES): // isFocus: 0
            RME_LOGI("[ReportWindowFocus]:%{public}d get focus", pid);
            break;
        case static_cast<int>(WindowState::FOCUS_NO): // isFocus: 1
            RME_LOGI("[ReportWindowFocus]:%{public}d lost focus", pid);
            break;
        default:
            RME_LOGI("[ReportWindowFocus]:unknown msg!");
            break;
    }
}

inline CgroupPolicy IntelliSenseServer::CheckCgroupState(CgroupPolicy cgroup)
{
    return ((cgroup == CgroupPolicy::SP_FOREGROUND) || (cgroup == CgroupPolicy::SP_TOP_APP)) ?
        CgroupPolicy::SP_FOREGROUND : CgroupPolicy::SP_BACKGROUND;
}

void IntelliSenseServer::ReportCgroupChange(const int pid, const int uid, const int oldGroup, const int newGroup)
{
    if (uid >= WEB_BASE_UID && uid <= WEB_END_UID) {
        return;
    }
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    CgroupPolicy oldState = CheckCgroupState(static_cast<CgroupPolicy>(oldGroup));
    CgroupPolicy newState = CheckCgroupState(static_cast<CgroupPolicy>(newGroup));
    if (oldState == newState) {
        return;
    }
    if (newState == CgroupPolicy::SP_BACKGROUND) {
        RME_LOGI("CgroupChange NewBackground");
        NewBackground(pid);
        AuthBackground(pid);
    } else if (newState == CgroupPolicy::SP_FOREGROUND) {
        RME_LOGI("web test CgroupChange NewForeground uid is %{public}d", uid);
        NewForeground(pid, uid);
        AuthForeground(pid);
    }
}

void IntelliSenseServer::ReportAppInfo(const int pid, const int uid, const std::string bundleName, ThreadState state)
{
    if (!m_switch) {
        return;
    }
    if (state == ThreadState::CREATE) {
        AuthForeground(pid);
    }
    RME_LOGI("Get app info:%{public}d %{public}d %{public}s %{public}d",
        pid, uid, bundleName.c_str(), static_cast<int>(state));
}

void IntelliSenseServer::ReportProcessInfo(const int pid,
    const int uid, const std::string bundleName, ThreadState state)
{
    if (uid >= WEB_BASE_UID && uid <= WEB_END_UID) {
        return;
    }
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    if (m_unsupportApp.find(bundleName) != m_unsupportApp.end()) {
        return;
    }
    switch (state) {
        case ThreadState::DIED:
            RME_LOGI("ProcessInfo NewDiedProcess");
            AuthAppKilled(pid);
            NewDiedProcess(pid);
            break;
        case ThreadState::CREATE:
            RME_LOGI("ProcessInfo NewAppRecord");
            NewAppRecord(pid, uid);
            break;
        default:
            RME_LOGI("unknown state : %{public}d", static_cast<int>(state));
            break;
    }
    return;
}

void IntelliSenseServer::SetPara(const int32_t currentFps, const int32_t currentRenderType)
{
    if (!m_switch) {
        return;
    }
    RME_LOGI("[SetPara]:ioctl SetPara!\n");
    std::string key = std::to_string(currentRenderType) + " " + std::to_string(currentFps);
    map<std::string, int> tempMap = m_subEventPara[key];
    RME_LOGI("[SetPara]:subEventPara map size: %{public}zu", tempMap.size());
}

void IntelliSenseServer::AuthAppKilled(int pid)
{
    int ret = AuthDelete(pid);
    if (ret == 0) {
        RME_LOGI("auth_delete %{public}d success", pid);
    } else {
        RME_LOGE("auth_delete %{public}d failed", pid);
    }
}

void IntelliSenseServer::AuthForeground(int pid)
{
    unsigned int flag = AF_RTG_ALL;
    int status = AUTH_STATUS_FOREGROUND;
    int ret = AuthEnable(pid, flag, status);
    if (ret == 0) {
        RME_LOGI("auth_enable %{public}d success", pid);
    } else {
        RME_LOGE("auth_enable %{public}d failed", pid);
    }
}

void IntelliSenseServer::AuthBackground(int pid)
{
    int ret = AuthPause(pid);
    if (ret == 0) {
        RME_LOGI("auth_pause %{public}d success", pid);
    } else {
        RME_LOGE("auth_pause %{public}d failed", pid);
    }
}

int IntelliSenseServer::CreateNewRtgGrp(int prioType, int rtNum)
{
    struct rtg_grp_data grp_data;
    int ret;
    char fileName[] = "/proc/self/sched_rtg_ctrl";
    int fd = open(fileName, O_RDWR);
    if (fd < 0) {
        RME_LOGE("Open file /proc/self/sched_rth_ctrl, errno = %{public}d", errno);
        return fd;
    }
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    if ((prioType > 0) && (prioType < RTG_TYPE_MAX)) {
        grp_data.prio_type = prioType;
    }
    if (rtNum > 0) {
        grp_data.rt_cnt = rtNum;
    }
    grp_data.rtg_cmd = CMD_CREATE_RTG_GRP;
    ret = ioctl(fd, CMD_ID_SET_RTG, &grp_data);
    if (ret < 0) {
        RME_LOGE("create rtg grp failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("create rtg grp success, get rtg id %{public}d.", ret);
    }
    close(fd);
    return ret;
}
} // namespace RME
} // namesapce OHOS
