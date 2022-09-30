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
#include <list>
#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>
#include <hitrace_meter.h>
#include "para_config.h"
#include "rtg_interface.h"
#include "rme_log_domain.h"

namespace OHOS {
namespace RME {
namespace {
    static std::string configFilePath = "/system/etc/frame_aware_sched/hwrme.xml"; // need To check the exact file path.
}
using namespace std;

DEFINE_RMELOG_INTELLISENSE("ueaServer-IntelliSenseServer");
IMPLEMENT_SINGLE_INSTANCE(IntelliSenseServer);

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

void IntelliSenseServer::NewForeground(int pid)
{
    HITRACE_METER(HITRACE_TAG_ACE);
    int newCreatedRtg = 0;
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppPid() == pid) {
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
            DestroyRtgGrp(grpId);
        }
    }
}

void IntelliSenseServer::NewAppRecord(int pid)
{
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppPid() == pid) {
            RME_LOGI("[NewAppRecord]pid %{public}d already exist.", pid);
            return;
        }
    }
    AppInfo *tempRecord = new AppInfo(pid);
    m_historyApp.push_back(*tempRecord);
    tempRecord->SetAppState(AppState::APP_FOREGROUND);
}

void IntelliSenseServer::NewDiedProcess(int pid)
{
    HITRACE_METER(HITRACE_TAG_ACE);
    RME_LOGI("[ReportMessage]pid %{public}d died.", pid);
    for (auto iter = m_historyApp.begin(); iter != m_historyApp.end(); iter++) {
        if (iter->GetAppPid() == pid) {
            int grpId = iter->GetRtgrp();
            if (grpId > 0) {
                DestroyRtgGrp(grpId);
            }
            m_historyApp.erase(iter++);
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
        if (ret != 0) {
            RME_LOGE("[OnFore]:add render thread fail! pid:%{public}d,rtg:%{public}d!ret:%{publid}d",
                renderTid, grpId, ret);
        }
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
        NewBackground(pid);
    } else if (newState == CgroupPolicy::SP_FOREGROUND) {
        NewForeground(pid);
    }
}

void IntelliSenseServer::ReportAppInfo(const int pid, const int uid, const std::string bundleName, ThreadState state)
{
    if (!m_switch) {
        return;
    }
    RME_LOGI("Get app info:%{public}d %{public}d %{public}s %{public}d",
        pid, uid, bundleName.c_str(), static_cast<int>(state));
}

void IntelliSenseServer::ReportProcessInfo(const int pid,
    const int uid, const std::string bundleName, ThreadState state)
{
    if (!m_switch) {
        return;
    }
    HITRACE_METER(HITRACE_TAG_ACE);
    if (m_unsupportApp.find(bundleName) != m_unsupportApp.end()) {
        return;
    }
    switch (state) {
        case ThreadState::DIED:
            NewDiedProcess(pid);
            break;
        case ThreadState::CREATE:
            NewAppRecord(pid);
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
} // namespace RME
} // namesapce OHOS
