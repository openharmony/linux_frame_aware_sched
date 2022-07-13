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
#include <map>
#include <new>
#include <utility>
#include "rme_log_domain.h"

namespace OHOS {
namespace RME {
DEFINE_RMELOG_INTELLISENSE("ueaClient-FrameMsgMgr");
IMPLEMENT_SINGLE_INSTANCE(FrameMsgMgr);

FrameMsgMgr::FrameMsgMgr()
    : sceneType(SceneEvent::SLIDE), rmeScene(nullptr)
{
    FrameMapKeyToFunc();
}

FrameMsgMgr::~FrameMsgMgr()
{
    if (rmeScene != nullptr) {
        delete rmeScene;
        rmeScene = nullptr;
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

void FrameMsgMgr::EventUpdate(FrameEvent event)
{
    switch (event) {
        case FrameEvent::EVENT_SET_PARAM:
            SetSchedParam();
            break;
        default:
            HandleFrameMsgKey(event);
            break;
    }
}

bool FrameMsgMgr::HandleFrameMsgKey(FrameEvent event)
{
    std::map<FrameEvent, PHandle>::iterator iter = m_frameMsgKeyToFunc.find(event);
    if (m_frameMsgKeyToFunc.end() == iter) {
        RME_LOGE("[HandleFrameMsgKey]: search frameEvent:%{public}d failed!",
            static_cast<int>(event));
        return false;
    }
    PHandle pFunction = iter->second;
    (this->*pFunction)();
    return true;
}

void FrameMsgMgr::FrameMapKeyToFunc()
{
    m_frameMsgKeyToFunc[FrameEvent::EVENT_BEGIN_FRAME] = &FrameMsgMgr::HandleBeginScene;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_ANIMATION_START] = &FrameMsgMgr::BeginFlushAnimation;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_ANIMATION_END] = &FrameMsgMgr::EndFlushAnimation;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_BUILD_START] = &FrameMsgMgr::BeginFlushBuild;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_BUILD_END] = &FrameMsgMgr::EndFlushBuild;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_LAYOUT_START] = &FrameMsgMgr::BeginFlushLayout;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_LAYOUT_END] = &FrameMsgMgr::EndFlushLayout;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_RENDER_START] = &FrameMsgMgr::BeginFlushRender;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_RENDER_END] = &FrameMsgMgr::EndFlushRender;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_RENDER_FINISH_START] = &FrameMsgMgr::BeginFlushRenderFinish;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_FLUSH_RENDER_FINISH_END] = &FrameMsgMgr::EndFlushRenderFinish;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_PROCESS_POST_FLUSH_START] = &FrameMsgMgr::BeginProcessPostFlush;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_PROCESS_COMMANDS_START] = &FrameMsgMgr::ProcessCommandsStart;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_ANIMATE_START] = &FrameMsgMgr::AnimateStart;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_RENDER_START] = &FrameMsgMgr::RenderStart;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_SEND_COMMANDS_START] = &FrameMsgMgr::SendCommandsStart;
    m_frameMsgKeyToFunc[FrameEvent::EVENT_END_FRAME] = &FrameMsgMgr::HandleEndScene;
}

FrameSceneSched *FrameMsgMgr::GetSceneHandler() const
{
    if (sceneType == SceneEvent::SCENE_INVALID) {
        RME_LOGE("[GetSceneHandler]:get nullptr sceneType %{public}d,", static_cast<int>(sceneType));
        return nullptr;
    }
    return rmeScene;
}

void FrameMsgMgr::UpdateScene(SceneEvent scene)
{
    if (scene < SceneEvent::SCENE_INVALID || scene >= SceneEvent::SCENE_MAX) {
        scene = SceneEvent::SCENE_INVALID;
    }
    sceneType = scene;
}

void FrameMsgMgr::HandleBeginScene()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[HandleBeginScene]:scene nullptr");
        return;
    }
    scene->HandleBeginScene();
}

void FrameMsgMgr::BeginFlushAnimation()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginFlushAnimation]:scene nullptr");
        return;
    }
    scene->BeginFlushAnimation();
}

void FrameMsgMgr::EndFlushAnimation()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[EndFlushAnimation]:scene nullptr");
        return;
    }
    scene->EndFlushAnimation();
}

void FrameMsgMgr::BeginFlushBuild()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginFlushBuild]:scene nullptr");
        return;
    }
    scene->BeginFlushBuild();
}

void FrameMsgMgr::EndFlushBuild()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[EndFlushBuild]:scene nullptr");
        return;
    }
    scene->EndFlushBuild();
}

void FrameMsgMgr::BeginFlushLayout()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginFlushLayout]:scene nullptr");
        return;
    }
    scene->BeginFlushLayout();
}

void FrameMsgMgr::EndFlushLayout()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[EndFlushLayout]:scene nullptr");
        return;
    }
    scene->EndFlushLayout();
}

void FrameMsgMgr::BeginFlushRender()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginFlushRender]:scene nullptr");
        return;
    }
    scene->BeginFlushRender();
}

void FrameMsgMgr::EndFlushRender()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[EndFlushRender]:scene nullptr");
        return;
    }
    scene->EndFlushRender();
}

void FrameMsgMgr::BeginFlushRenderFinish()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginFlushRenderFinish]:scene nullptr");
        return;
    }
    scene->BeginFlushRenderFinish();
}

void FrameMsgMgr::EndFlushRenderFinish()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[EndFlushRenderFinish]:scene nullptr");
        return;
    }
    scene->EndFlushRenderFinish();
}

void FrameMsgMgr::BeginProcessPostFlush()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[BeginProcessPostFlush]:scene nullptr");
        return;
    }
    scene->BeginProcessPostFlush();
}

void FrameMsgMgr::ProcessCommandsStart()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[ProcessCommandsStart]:scene nullptr");
        return;
    }
    scene->ProcessCommandsStart();
}

void FrameMsgMgr::AnimateStart()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[AnimateStart]:scene nullptr");
        return;
    }
    scene->AnimateStart();
}

void FrameMsgMgr::RenderStart()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[RenderStart]:scene nullptr");
        return;
    }
    scene->RenderStart();
}

void FrameMsgMgr::SendCommandsStart()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[SendCommandsStart]:scene nullptr");
        return;
    }
    scene->SendCommandsStart();
}

void FrameMsgMgr::HandleEndScene()
{
    FrameSceneSched *scene = GetSceneHandler();
    if (scene == nullptr) {
        RME_LOGE("[HandleEndScene]:scene nullptr");
        return;
    }
    scene->HandleEndScene();
}

void FrameMsgMgr::SetSchedParam()
{
    RME_LOGI("[SetSchedParam]: set default sched param!");
}
} // namespace RME
} // namespace OHOS
