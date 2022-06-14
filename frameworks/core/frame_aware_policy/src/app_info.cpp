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

#include "app_info.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr int RTG_GRP_INIT = -1;
}

AppInfo::AppInfo(std::string appName, int pid, int uiTid, int renderTid, int isFocus, AppState state)
    : m_appName(appName),
      m_pid(pid),
      m_uiTid(uiTid),
      m_renderTid(renderTid),
      m_isFocus(isFocus),
      m_rtGrp(RTG_GRP_INIT),
      m_appState(state)
{}

AppInfo::AppInfo(int pid)
    : m_appName(""),
      m_pid(pid),
      m_uiTid(0),
      m_renderTid(0),
      m_isFocus(0),
      m_rtGrp(0),
      m_appState(AppState::APP_TERMINATED)
{}

void AppInfo::SetRenderTid(const int tid)
{
    this->m_renderTid = tid;
}

int AppInfo::GetRenderTid()
{
    return m_renderTid;
}

void AppInfo::SetUiTid(const int tid)
{
    this->m_uiTid = tid;
}

int AppInfo::GetUiTid()
{
    return m_uiTid;
}

void AppInfo::SetAppName(const std::string appName)
{
    this->m_appName = appName;
}

void AppInfo::SetFocusState(const int isFocus)
{
    this->m_isFocus = isFocus;
}

int AppInfo::GetFocusState()
{
    return m_isFocus;
}

void AppInfo::SetAppState(AppState appState)
{
    this->m_appState = appState;
}

void AppInfo::SetAppPid(const int pid)
{
    this->m_pid = pid;
}

int AppInfo::GetAppPid()
{
    return m_pid;
}

AppState AppInfo::GetAppState()
{
    return m_appState;
}

void AppInfo::SetRtgrp(const int grpNum)
{
    this->m_rtGrp = grpNum;
}

int AppInfo::GetRtgrp()
{
    return m_rtGrp;
}
} // namespace RME
} // namespace OHOS
