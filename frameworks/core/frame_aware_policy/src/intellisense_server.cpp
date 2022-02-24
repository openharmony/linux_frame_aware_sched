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

#include <hilog/log.h>

#include "para_config.h"
#include "rtg_msg_mgr.h"
#include "rme_log_domain.h"
#include "app_info_mgr.h"

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
    RtgMsgMgr::GetInstance().Init();
    RME_LOGI("[Init]:Init rtg and readXml finish!");
}

bool IntelliSenseServer::ReadXml()
{
    if (!m_needReadXml) { // do this value really need?
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

void IntelliSenseServer::ReportMessage(std::string appName, std::string processName, int pid, AppStateUpdateReason reason)
{
    int rtGrp = AppInfoMgr::GetInstance().GetAppRtgrp(pid);
    switch (reason) {
        case AppStateUpdateReason::APP_FOREGROUND:
            {
                rtGrp = RtgMsgMgr::GetInstance().OnForeground(appName, pid);
                AppInfoMgr::GetInstance().OnForegroundChanged(pid, appName, rtGrp);
                RME_LOGI("[ReportMessage]: App_foreground! rtGrp: %{public}d", rtGrp);
            }
            break;
        case AppStateUpdateReason::APP_BACKGROUND:
            {
                RtgMsgMgr::GetInstance().OnBackground(appName, pid, rtGrp);
                AppInfoMgr::GetInstance().OnBackgroundChanged(pid, appName);
                RME_LOGI("[ReportMessage]: App_background! rtGrp: %{public}d", rtGrp);
            }
            break;
        case AppStateUpdateReason::APP_TERMINATED:
            {
                RtgMsgMgr::GetInstance().ProcessDied(pid, -1);
                AppInfoMgr::GetInstance().OnAppTerminateChanged(pid, appName);
                RME_LOGI("[ReportMessage]: App terminated! rtGrp: %{public}d", rtGrp);
            }
            break;
        default:
            RME_LOGI("[ReportMessage]: get unuse app state msg!");
            break;
    }
    return;
}

void IntelliSenseServer::ReportWindowFocus(const int pid, int isFocus)
{
    int rtGrp = AppInfoMgr::GetInstance().GetAppRtgrp(pid);
    switch (isFocus) {
        case static_cast<int>(WindowState::FOCUS_YES): // isFocus: 0
            {
                rtGrp = RtgMsgMgr::GetInstance().OnForeground("", pid);
                AppInfoMgr::GetInstance().OnForegroundChanged(pid, "", rtGrp);
                RME_LOGI("[ReportWindowFocus]: Focus yes!rtGrp: %{public}d", rtGrp);
            }
            break;
        case static_cast<int>(WindowState::FOCUS_NO): // isFocus: 1
            {
                RtgMsgMgr::GetInstance().OnBackground("", pid, rtGrp);
                AppInfoMgr::GetInstance().OnBackgroundChanged(pid, "");
                RME_LOGI("[ReportWindowFocus]: Focus No!rtGrp: %{public}d", rtGrp);
            }
            break;
        default:
            RME_LOGI("[ReportWindowFocus]:unknown msg!");
            break;
    }
    AppInfoMgr::GetInstance().OnWindowFocus(pid, isFocus);
    RtgMsgMgr::GetInstance().FocusChanged(pid, isFocus);
}

void IntelliSenseServer::ReportProcessInfo(const int pid, const int tid, ThreadState state)
{
    switch (state) {
        case ThreadState::DIED:
            {
                int ret = AppInfoMgr::GetInstance().OnProcessDied(pid, tid);
                if (ret) {
                    RtgMsgMgr::GetInstance().ProcessDied(pid, tid);
                    RME_LOGI("process died, need to delete the rtgrp:pid:%{public}d, tid: %{public}d, \
                        threadstate: %{public}d", pid, tid, static_cast<int>(state));
                }
                RME_LOGI("process died, do not need to delete the rtgrp:pid:%{public}d, tid: %{public}d",
                    pid, tid);
            }
            break;
        case ThreadState::CREATE:
            RME_LOGI("process create, pid: %{public}d, tid: %{public}d, threadstate: %{public}d",
                pid, tid, static_cast<int>(state));
            break;
        default:
            RME_LOGI("unknown state : %{public}d", static_cast<int>(state));
            break;
    }
    return;
}

void IntelliSenseServer::SetPara(const int32_t currentFps, const int32_t currentRenderType)
{
    RME_LOGI("[SetPara]:ioctl SetPara!\n");
    std::string key = std::to_string(currentRenderType) + " " + std::to_string(currentFps);
    map<std::string, int> tempMap = m_subEventPara[key];
    RME_LOGI("[SetPara]:subEventPara map size: %{public}d", tempMap.size());
}
} // namespace RME
} // namesapce OHOS
