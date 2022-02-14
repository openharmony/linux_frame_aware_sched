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

DEFINE_RMELOG_SERVICE("ueaServer-AppInfoMgr");

namespace {
    constexpr int INIT_VAL = -1;
}

IMPLEMENT_SINGLE_INSTANCE(AppInfoMgr);

void AppInfoMgr::OnForegroundChanged(const int pid, const std::string appName, const int rtGrp)
{
    RME_LOGI("[OnForegroundChanged]:pid %{public}d, appname:%{public}s", pid, appName.c_str());
    std::map<int, std::shared_ptr<AppInfo>>::iterator it = mForegroundAppList.find(pid);
    if (it != mForegroundAppList.end()) {
        mForegroundAppList[pid]->SetAppName(appName);
        mForegroundAppList[pid]->SetAppState(AppState::APP_FOREGROUND);
    } else {
        std::map<int, std::shared_ptr<AppInfo>>::iterator itB = mBackgroundAppList.find(pid);
        if (itB != mBackgroundAppList.end()) {
            mBackgroundAppList[pid]->SetAppName(appName);
            mBackgroundAppList[pid]->SetAppState(AppState::APP_FOREGROUND);
            mForegroundAppList[pid] = mBackgroundAppList[pid];
            mBackgroundAppList.erase(pid);
        } else {
            auto appInfo = std::make_shared<AppInfo>(appName, pid, INIT_VAL, INIT_VAL, INIT_VAL, AppState::APP_FOREGROUND);
            mForegroundAppList[pid] = appInfo;
        }
    }
    mForegroundAppList[pid]->SetRtgrp(rtGrp);
    RME_LOGI("[OnForegroundChanged]rtgrp:%{public}d:", rtGrp);
}

void AppInfoMgr::OnBackgroundChanged(const int pid, const std::string appName)
{
    RME_LOGI("[OnBackgroundChanged]:pid:%{public}d, appName:%{public}s", pid, appName.c_str());
    std::map<int, std::shared_ptr<AppInfo>>::iterator it = mForegroundAppList.find(pid);
    if (it != mForegroundAppList.end()) {
        mForegroundAppList[pid]->SetAppState(AppState::APP_BACKGROUND);
        mBackgroundAppList[pid] = mForegroundAppList[pid];
        mForegroundAppList.erase(pid);
    } else {
        RME_LOGE("[OnBackgroundChanged]:unfind appName in foreground app when go to background!");
    }
}

void AppInfoMgr::OnAppTerminateChanged(const int pid, const std::string appName)
{
    RME_LOGI("[OnAppTerminatedChanged]: pid: %{public}d, appName: %{public}s:", pid, appName.c_str());
    mForegroundAppList.erase(pid);
    mBackgroundAppList.erase(pid);
}

void AppInfoMgr::OnWindowFocus(const int pid, bool isFocus)
{
    RME_LOGI("[OnAppFocus]: pid:%{public}d, isFocus:%{public}d", pid, isFocus);
    std::shared_ptr<AppInfo> appInfo = nullptr; 
    if (mForegroundAppList.find(pid) != mForegroundAppList.end()) {
        appInfo = mForegroundAppList[pid];
        mForegroundAppList.erase(pid);
    } else if (mBackgroundAppList.find(pid) != mBackgroundAppList.end()) {
        appInfo = mBackgroundAppList[pid];
        mBackgroundAppList.erase(pid);
    } else {
        appInfo = std::make_shared<AppInfo>("", pid, INIT_VAL, INIT_VAL, isFocus, AppState::APP_FOREGROUND);
    }
    appInfo->SetFocusState(isFocus);
    if (isFocus) {
        mForegroundAppList[pid] = appInfo;
        SetFocusApp(pid);
    } else {
        mBackgroundAppList[pid] = appInfo;
    }
}

void AppInfoMgr::OnUiProcessStart(const int pid, const int tid)
{
    RME_LOGI("[OnUiProcessStart]:pid:%{public}d, uitid:%{public}d.\n", pid, tid);
    std::shared_ptr<AppInfo> appInfo = nullptr; 
    if (mForegroundAppList.find(pid) != mForegroundAppList.end()) {
        appInfo = mForegroundAppList[pid];
        mForegroundAppList.erase(pid);
    } else if (mBackgroundAppList.find(pid) != mBackgroundAppList.end()) {
        appInfo = mBackgroundAppList[pid];
        mBackgroundAppList.erase(pid);
    } else {
        appInfo = std::make_shared<AppInfo>("", pid, tid, INIT_VAL, INIT_VAL, AppState::APP_UNKNOWN);
    }
    appInfo->SetUiTid(tid);
}

bool AppInfoMgr::OnProcessDied(const int pid, const int tid)
{
    bool deleted = false;
    if (mForegroundAppList.count(pid)) {
        mForegroundAppList.erase(pid);
        deleted = true;
    } else if (mBackgroundAppList.count(pid)) {
        mBackgroundAppList.erase(pid);
    }
    RME_LOGI("[OnProcessDied]: pid: %{public}d, tid:%{public}d, deleted:%{public}d.\n", pid, tid, deleted);
    return deleted;
}

void AppInfoMgr::OnRenderProcessStart(const int pid, const int tid)
{
    RME_LOGI("[OnRenderProcessStart]:pid:%{public}d, tid:%{public}d.\n", pid, tid);
    std::shared_ptr<AppInfo> appInfo = nullptr; 
    if (mForegroundAppList.find(pid) != mForegroundAppList.end()) {
        appInfo = mForegroundAppList[pid];
        mForegroundAppList.erase(pid);
    } else if (mBackgroundAppList.find(pid) != mBackgroundAppList.end()) {
        appInfo = mBackgroundAppList[pid];
        mBackgroundAppList.erase(pid);
    } else {
        appInfo = std::make_shared<AppInfo>("", pid, INIT_VAL, tid, INIT_VAL, AppState::APP_UNKNOWN);
    }
    appInfo->SetUiTid(tid);
}

void AppInfoMgr::SetFocusApp(const int pid)
{
    mFocusApp = mForegroundAppList[pid];
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
        RME_LOGI("[GetAppRtgrp]: do not have this pid, please add!");
    }
    return rtGrp;
}

std::map<int, std::shared_ptr<AppInfo>> AppInfoMgr::GetForegroundApp() const
{
    return mForegroundAppList;
}

} // namespace RME
} // namespace OHOS
