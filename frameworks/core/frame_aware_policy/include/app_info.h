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

#ifndef APP_INFO_H
#define APP_INFO_H

#include <string>
#include <iosfwd>

namespace OHOS {
namespace RME {
namespace {
    constexpr int RT_PRIO = 0;
    constexpr int RT_NUM = 4;
}

enum class AppState {
    APP_FOREGROUND,
    APP_FOREGROUND_WITHOUT_RTG,
    APP_BACKGROUND,
    APP_TERMINATED,
    APP_UNKNOWN,
};

enum class CgroupPolicy {
    SP_DEFAULT = 0,
    SP_BACKGROUND = 1,
    SP_FOREGROUND = 2,
    SP_SYSTEM_BACKGROUND = 3,
    SP_TOP_APP = 4,
    SP_SYSTEM_DEFAULT = SP_DEFAULT
};

class AppInfo {
public:
    AppInfo(std::string appName, int pid, int uiTid, int renderTid, int isFocus, AppState state);
    AppInfo(int pid);
    ~AppInfo() = default;

    void SetRenderTid(const int tid);
    int GetRenderTid();
    void SetUiTid(const int tid);
    int GetUiTid();
    void SetAppName(const std::string appName);
    void SetAppState(AppState appState);
    AppState GetAppState();
    void SetFocusState(const int isFocus);
    int GetFocusState();
    void SetAppPid(const int pid);
    int GetAppPid();
    void SetRtgrp(const int grpNum);
    int GetRtgrp();
private:
    std::string m_appName;
    int m_pid;
    int m_uiTid;
    int m_renderTid;
    int m_isFocus;
    int m_rtGrp;
    AppState m_appState;
};
} // namespace RME
} // namespace OHOS
#endif
