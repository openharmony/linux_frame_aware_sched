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

#ifndef FRAME_SCENE_SCHED_H
#define FRAME_SCENE_SCHED_H

#include "rme_log_domain.h"

namespace OHOS {
namespace RME {
class FrameSceneSched {
public:
    FrameSceneSched() {};
    virtual ~FrameSceneSched() = default;

    virtual void HandleBeginScene() = 0;
    virtual void HandleEndScene() = 0;
    virtual void BeginFlushAnimation() = 0;
    virtual void EndFlushAnimation() = 0;
    virtual void BeginFlushBuild() = 0;
    virtual void EndFlushBuild() = 0;
    virtual void BeginFlushLayout() = 0;
    virtual void EndFlushLayout() = 0;
    virtual void BeginFlushRender() = 0;
    virtual void EndFlushRender() = 0;
    virtual void BeginFlushRenderFinish() = 0;
    virtual void EndFlushRenderFinish() = 0;
    virtual void BeginProcessPostFlush() = 0;
    virtual void ProcessCommandsStart() = 0;
    virtual void AnimateStart() = 0;
    virtual void RenderStart() = 0;
    virtual void SendCommandsStart() = 0;
};
} // namespace RME
} // namespace OHOS
#endif
