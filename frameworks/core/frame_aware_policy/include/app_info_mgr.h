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

#ifndef APP_INFO_MGR_H
#define APP_INFO_MGR_H

#include <map>

#include "app_info.h"
#include "rme_log_domain.h"
#include "single_instance.h"

namespace OHOS {
namespace RME {

class AppInfoMgr {
    DECLARE_SINGLE_INSTANCE(AppInfoMgr);

public:
    std::map<int, std::shared_ptr<AppInfo>> GetForegroundApp() const;
    void OnForegroundChanged(const int pid, const std::string appName, const int rtGrp);
    void OnBackgroundChanged(const int pid, const std::string appName);
    void OnAppTerminateChanged(const int pid, const std::string appName);
    void OnWindowFocus(const int pid, bool isFocus);
    void OnUiProcessStart(const int pid, const int tid);
    void OnRenderProcessStart(const int pid, const int tid);
    bool OnProcessDied(const int pid, const int tid);
    std::shared_ptr<AppInfo> GetFocusApp() const;
    void SetFocusApp(const int pid);
    int GetAppRtgrp(const int pid);

private:
    std::map<int, std::shared_ptr<AppInfo>> mForegroundAppList;
    std::map<int, std::shared_ptr<AppInfo>> mBackgroundAppList;
    std::shared_ptr<AppInfo> mFocusApp;
};


} // namespace RME
} // namespace OHOS
#endif
