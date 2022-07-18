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

#ifndef FRAME_MSG_MGR_H
#define FRAME_MSG_MGR_H

#include <map>
#include "frame_info_const.h"
#include "single_instance.h"
#include "frame_scene_sched.h"
#include "rme_scene_sched.h"

namespace OHOS {
namespace RME {
class FrameMsgMgr {
    DECLARE_SINGLE_INSTANCE_BASE(FrameMsgMgr);
public:
    FrameMsgMgr();
    ~FrameMsgMgr();
    bool Init();
    void EventUpdate(FrameEvent eventType);
private:
    bool HandleFrameMsgKey(FrameEvent event);
    void FrameMapKeyToFunc();
    void UpdateScene(SceneEvent scene);
    void SetSchedParam();

    void HandleBeginScene();
    void BeginFlushAnimation();
    void EndFlushAnimation();
    void BeginFlushBuild();
    void EndFlushBuild();
    void BeginFlushLayout();
    void EndFlushLayout();
    void BeginFlushRender();
    void EndFlushRender();
    void BeginFlushRenderFinish();
    void EndFlushRenderFinish();

    void BeginProcessPostFlush();
    void ProcessCommandsStart();
    void AnimateStart();
    void RenderStart();
    void SendCommandsStart();
    void HandleEndScene();

    FrameSceneSched *GetSceneHandler() const;
    typedef void (FrameMsgMgr:: *PHandle)(void);
    std::map<FrameEvent, PHandle> m_frameMsgKeyToFunc;

    SceneEvent sceneType;
    RmeSceneSched *rmeScene;
};
} // namespace RME
} // namespace OHOS
#endif
