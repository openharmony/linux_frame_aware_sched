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

#include "frame_window_mgr.h"
#include <iosfwd>
#include <string>
#include "rme_log_domain.h"

namespace OHOS {
namespace RME {
namespace {
    const std::string SCHEME_SWITCH_PROP = ""; // to add prop
}

IMPLEMENT_SINGLE_INSTANCE(FrameWindowMgr);
DEFINE_RMELOG_INTELLISENSE("ueaClient-FrameWindowMgr");

int FrameWindowMgr::GetEnable()
{
    if (m_enable == -1) {
        m_enable = 1;
        RME_LOGI("[GetEnable]: m_enable: %{public}d", m_enable);
    }
    return m_enable;
}

void FrameWindowMgr::SetStartFlag(bool flag)
{
    if (flag) {
        RME_LOGI("fling start");
    } else {
        RME_LOGI("fling end");
    }
    m_startFlag = flag;
}

bool FrameWindowMgr::GetStartFlag()
{
    return m_startFlag;
}
} // namespace RME
} // namespace OHOS
