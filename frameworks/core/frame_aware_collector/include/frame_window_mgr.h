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

#ifndef FRAME_WINDOW_MGR_H
#define FRAME_WINDOW_MGR_H

#include "single_instance.h"

namespace OHOS {
namespace RME {
class FrameWindowMgr {
    DECLARE_SINGLE_INSTANCE(FrameWindowMgr);
public:
    int GetEnable();
    void SetStartFlag(bool flag);
    bool GetStartFlag();
private:
    int m_enable = -1;
    bool m_startFlag;
};
} // namespace RME
} // namespace OHOS
#endif
