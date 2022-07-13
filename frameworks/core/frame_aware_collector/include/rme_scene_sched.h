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

#ifndef RME_SCENE_SCHED_H
#define RME_SCENE_SCHED_H

#include "rme_core_sched.h"
#include "frame_scene_sched.h"

namespace OHOS {
namespace RME {
class RmeSceneSched : public FrameSceneSched {
public:
    RmeSceneSched();
    ~RmeSceneSched() override;

    bool Init();
    void HandleBeginScene() override;
    void HandleEndScene() override;

    void BeginFlushAnimation() override;
    void EndFlushAnimation() override;

    void BeginFlushBuild() override;
    void EndFlushBuild() override;

    void BeginFlushLayout() override;
    void EndFlushLayout() override;

    void BeginFlushRender() override;
    void EndFlushRender() override;

    void BeginFlushRenderFinish() override;
    void EndFlushRenderFinish() override;

    void BeginProcessPostFlush() override;
    void ProcessCommandsStart() override;
    void AnimateStart() override;
    void RenderStart() override;
    void SendCommandsStart() override;
private:
    int curWorkingStatus;
    RmeCoreSched *rmeCoreSched;
};
} // namespace RME
} // namespace OHOS
#endif
