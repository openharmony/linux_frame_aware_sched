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

#include "frame_msg_mgr.h"

namespace OHOS {
namespace RME {
DEFINE_RMELOG_INTELLISENSE("ueaClient-FrameMsgMgr");
IMPLEMENT_SINGLE_INSTANCE(FrameMsgMgr);

FrameMsgMgr::FrameMsgMgr()
    : sceneType(SceneEvent::SLIDE), rmeScene(nullptr)
{}

FrameMsgMgr::~FrameMsgMgr()
{
    if (rmeScene != nullptr) {
        delete rmeScene;
    }
}

bool FrameMsgMgr::Init()
{
    if (rmeScene == nullptr) {
        rmeScene = new RmeSceneSched();
    }
    if (!rmeScene->Init()) {
        RME_LOGE("[Init]:inited failed!");
        return false;
    }
    RME_LOGI("[Init]:inited success!");
    return true;
}

void FrameMsgMgr::EventUpdate(FrameEvent event, EventState value)
{
    switch (event) {
        case FrameEvent::EVENT_SET_PARAM:
            SetSchedParam();
            break;
        default:
            HandleDefaultEvent(event, value);
            break;    
    }
    return;
}

void FrameMsgMgr::UpdateScene(SceneEvent scene)
{
    if (scene < SceneEvent::SCENE_INVALID || scene >= SceneEvent::SCENE_MAX) {
        scene = SceneEvent::SCENE_INVALID;
    }
    sceneType = scene;
}

void FrameMsgMgr::HandleDefaultEvent(FrameEvent event, EventState value)
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[HandleDefaultEvent]:scene nullptr");
        return;
    }
    switch (event) {
        case FrameEvent::FLUSH_ANIMATION:
            if (value == EventState::EVENT_ENTER) {
                scene->BeginFlushAnimation();
            } else {
                scene->EndFlushAnimation();
            }
            break;
        case FrameEvent::FLUSH_BUILD:
            if (value == EventState::EVENT_ENTER) {
                scene->BeginFlushBuild();
            } else {
                scene->EndFlushBuild();
            }
            break;
        case FrameEvent::FLUSH_LAYOUT:
            if (value == EventState::EVENT_ENTER) {
                scene->BeginFlushLayout();
            } else {
                scene->EndFlushLayout();
            }
            break;
        case FrameEvent::FLUSH_RENDER:
            if (value == EventState::EVENT_ENTER) {
                scene->BeginFlushRender();
            } else {
                scene->EndFlushRender();
            }
            break;
        case FrameEvent::FLUSH_RENDER_FINISH:
            if (value == EventState::EVENT_ENTER) {
                scene->BeginFlushRenderFinish();
            } else {
                scene->EndFlushRenderFinish();
            }
        case FrameEvent::PROCESS_POST_FLUSH:
            scene->BeginProcessPostFlush();
            break;
        case FrameEvent::PROCESS_COMMANDS:
            scene->ProcessCommandsStart();
            break;
        case FrameEvent::ANIMATE:
            scene->AnimateStart();
            break;
        case FrameEvent::RENDER:
            scene->RenderStart();
            break;
        case FrameEvent::SEND_COMMANDS:
            scene->SendCommandsStart();
            break;
        case FrameEvent::EVENT_END_FRAME:
            scene->HandleEndFrame();
            break;
        case FrameEvent::EVENT_BEGIN_FRAME:
            scene->HandleBeginFrame();
            break;
        default:
            RME_LOGE("[HandleDefaultEvent]: unknown event id: %{public}d, event state:%{public}d\n",
                static_cast<int>(event), static_cast<int>(value));
            return;
    }
}

void FrameMsgMgr::SetSchedParam()
{
    RME_LOGI("[SetSchedParam]: set default sched param!");
}

FrameSceneSched *FrameMsgMgr::GetSceneHandler() const
{
    if (sceneType == SceneEvent::SCENE_INVALID) {
        RME_LOGE("[GetSceneHandler]:get nullptr sceneType %{public}d,", static_cast<int>(sceneType));
        return nullptr;
    }
    return rmeScene;
}
} // namespace RME
} // namespace OHOS
