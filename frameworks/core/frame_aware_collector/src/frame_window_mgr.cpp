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
        bool prop = true;
        if (prop) {
            m_enable = 1;
            prop = false;
        } else {
            m_enable = 0;
        }
        RME_LOGI("[GetEnable]: prop: %{public}d", prop);
    }
    return m_enable;
}

void FrameWindowMgr::SetStartFlag(bool flag)
{
    m_startFlag = flag;
}

bool FrameWindowMgr::GetStartFlag()
{
    return m_startFlag;
}

} // namespace RME
} // namespace OHOS
