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

#include "rme_scene_sched.h"
#include <hitrace_meter.h>
#include "frame_window_mgr.h"

namespace OHOS {
namespace RME {
RmeSceneSched::RmeSceneSched()
    :curWorkingStatus(0), rmeCoreSched(nullptr)
{}

RmeSceneSched::~RmeSceneSched()
{
    if (rmeCoreSched != nullptr) {
        delete rmeCoreSched;
        rmeCoreSched = nullptr;
    }
}

bool RmeSceneSched::Init()
{
    if (rmeCoreSched == nullptr) {
        rmeCoreSched = new RmeCoreSched();
    }

    if (rmeCoreSched == nullptr) {
        return false;
    }
    bool ret = rmeCoreSched->Init();
    return ret;
}

void RmeSceneSched::HandleBeginScene()
{
    curWorkingStatus = 1;
    FrameWindowMgr::GetInstance().SetStartFlag(true);
    rmeCoreSched->HandleBeginScene();
    StartTrace(HITRACE_TAG_ACE, ("FrameS-curWorkingStatus" + std::to_string(curWorkingStatus)).c_str());
    FinishTrace(HITRACE_TAG_ACE);
}

void RmeSceneSched::BeginFlushAnimation()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushAnimation();
    }
}

void RmeSceneSched::EndFlushAnimation()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->EndFlushAnimation();
    }
}

void RmeSceneSched::BeginFlushBuild()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushBuild();
    }
}

void RmeSceneSched::EndFlushBuild()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->EndFlushBuild();
    }
}

void RmeSceneSched::BeginFlushLayout()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushLayout();
    }
}

void RmeSceneSched::EndFlushLayout()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->EndFlushLayout();
    }
}

void RmeSceneSched::BeginFlushRender()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushRender();
    }
}

void RmeSceneSched::EndFlushRender()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->EndFlushRender();
    }
}

void RmeSceneSched::BeginFlushRenderFinish()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushRenderFinish();
    }
}

void RmeSceneSched::EndFlushRenderFinish()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->EndFlushRenderFinish();
    }
}

void RmeSceneSched::BeginProcessPostFlush()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginProcessPostFlush();
    }
}

void RmeSceneSched::ProcessCommandsStart()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->ProcessCommandsStart();
    }
}

void RmeSceneSched::AnimateStart()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->AnimateStart();
    }
}

void RmeSceneSched::RenderStart()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->RenderStart();
    }
}

void RmeSceneSched::SendCommandsStart()
{
    if (curWorkingStatus == 1) {
        rmeCoreSched->SendCommandsStart();
    }
}

void RmeSceneSched::HandleEndScene()
{
    FrameWindowMgr::GetInstance().SetStartFlag(false);
    if (curWorkingStatus == 0) {
        return;
    }
    rmeCoreSched->HandleEndScene();
    curWorkingStatus = 0;
    StartTrace(HITRACE_TAG_ACE, ("FrameS-curWorkingStatus" + std::to_string(curWorkingStatus)).c_str());
    FinishTrace(HITRACE_TAG_ACE);
}
} // namespace RME
} // namespace OHOS
