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

#include "app_info_mgr.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr int INIT_VAL = -1;
}
DEFINE_RMELOG_SERVICE("ueaServer-AppInfoMgr");
IMPLEMENT_SINGLE_INSTANCE(AppInfoMgr);

void AppInfoMgr::OnForegroundChanged(const int pid, const std::string appName, const int rtGrp)
{
    if (mBackgroundAppList.count(pid)) {
        RME_LOGI("[OnForegroundChanged]: back yes!pid:%{public}d", pid);
        mBackgroundAppList[pid]->SetAppName(appName);
        mBackgroundAppList[pid]->SetAppState(AppState::APP_FOREGROUND);
        mForegroundAppList[pid] = mBackgroundAppList[pid];
        mBackgroundAppList.erase(pid);
    } else {
        RME_LOGI("[OnForegroundChanged]: back No!pid:%{public}d", pid);
        auto appInfo = std::make_shared<AppInfo>(appName, pid, INIT_VAL, INIT_VAL, INIT_VAL, AppState::APP_FOREGROUND);
        mForegroundAppList[pid] = appInfo;
    }
    mForegroundAppList[pid]->SetRtgrp(rtGrp);
    RME_LOGI("[OnForegroundChanged]:pid:%{public}d, rtgrp:%{public}d", pid, rtGrp);
}

void AppInfoMgr::OnBackgroundChanged(const int pid, const std::string appName)
{
    std::map<int, std::shared_ptr<AppInfo>>::iterator it = mForegroundAppList.find(pid);
    if (it != mForegroundAppList.end()) {
        mForegroundAppList[pid]->SetAppState(AppState::APP_BACKGROUND);
        mForegroundAppList[pid]->SetRtgrp(INIT_VAL);
        mBackgroundAppList[pid] = mForegroundAppList[pid];
        mForegroundAppList.erase(pid);
    } else {
        RME_LOGE("[OnBackgroundChanged]:unfind app in foreground app when go to background! pid:%{public}d", pid);
    }
    mBackgroundAppList[pid]->SetRtgrp(INIT_VAL);
    RME_LOGI("[OnBackgroundChanged]:pid:%{public}d, appName:%{public}s", pid, appName.c_str());
}

void AppInfoMgr::OnAppTerminateChanged(const int pid, const std::string appName)
{
    mForegroundAppList.erase(pid);
    mBackgroundAppList.erase(pid);
    RME_LOGI("[OnAppTerminatedChanged]: pid: %{public}d, appName: %{public}s, \
        ", pid, appName.c_str());
}

void AppInfoMgr::OnWindowFocus(const int pid, bool isFocus)
{
    // isFocus: 0:focused, 1:unfocused.
    std::shared_ptr<AppInfo> appInfo = nullptr;
    if (!isFocus) {
        appInfo = mForegroundAppList[pid];
        if (appInfo == nullptr) {
            RME_LOGE("[OnAppFocus]:not found pid %{public}d in fore map", pid);
            return;
        }
    } else {
        appInfo = mBackgroundAppList[pid];
        if (appInfo == nullptr) {
            RME_LOGE("[OnAppFocus]:not found pid %{public}d in back map", pid);
            return;
        }
    }
    SetFocusApp(pid, isFocus);
    appInfo->SetFocusState(isFocus);
    RME_LOGI("[OnAppFocus]: pid:%{public}d, isFocus:%{public}d, \
        ", pid, isFocus);
}

bool AppInfoMgr::OnProcessDied(const int pid, const int tid)
{
    // TO DO: need to add tid set mgr. now has process then erase.
    bool deleted = false;
    bool isUi = pid == tid ? true : false;
    if (mForegroundAppList.count(pid)) {
        if (isUi) {
            mForegroundAppList.erase(pid);
        } else {
            // TO DO: if render died, do not need process.
            mForegroundAppList[pid]->SetRenderTid(INIT_VAL);
        }
        deleted = true;
    } else if (mBackgroundAppList.count(pid)) {
        if (isUi) {
            mBackgroundAppList.erase(pid);
        } else {
            // TO DO: if render died, do not need process.
            mBackgroundAppList[pid]->SetRenderTid(INIT_VAL);
        }
    }
    RME_LOGI("[OnProcessDied]: pid: %{public}d, tid:%{public}d, deleted:%{public}d.", pid, tid, deleted);
    return deleted;
}

void AppInfoMgr::SetFocusApp(const int pid, bool isFocus)
{
    if (!isFocus) {
        mFocusApp = mForegroundAppList[pid];
    } else {
        mFocusApp = nullptr;
    }
    RME_LOGI("[SetFocusApp]:pid: %{public}d, isFocus%{public}d", pid, isFocus);
}

std::shared_ptr<AppInfo> AppInfoMgr::GetFocusApp() const
{
    return mFocusApp;
}

int AppInfoMgr::GetAppRtgrp(const int pid)
{
    int rtGrp = -1;
    if (mForegroundAppList.count(pid) != 0) {
        rtGrp = mForegroundAppList[pid]->GetRtgrp();
    } else if (mBackgroundAppList.count(pid) != 0) {
        rtGrp = mBackgroundAppList[pid]->GetRtgrp();
    } else {
        RME_LOGE("[GetAppRtgrp]: do not have this pid, please add!");
    }
    return rtGrp;
}

std::map<int, std::shared_ptr<AppInfo>> AppInfoMgr::GetForegroundApp() const
{
    return mForegroundAppList;
}
} // namespace RME
} // namespace OHOS
