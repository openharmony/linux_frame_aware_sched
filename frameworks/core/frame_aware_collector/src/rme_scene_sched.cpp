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

namespace OHOS {
namespace RME {

DEFINE_RMELOG_INTELLISENSE("ueaClient-RmeSceneSched");

RmeSceneSched::RmeSceneSched()
    :curWorkingStatus(0), rmeCoreSched(nullptr)
{
}

RmeSceneSched::~RmeSceneSched()
{
    if (rmeCoreSched != nullptr) {
        delete rmeCoreSched;
    }
}

bool RmeSceneSched::Init()
{
    if (rmeCoreSched == nullptr) {
        rmeCoreSched = new RmeCoreSched();
    }

    curWorkingStatus = 1;
    if (rmeCoreSched == nullptr) {
        return false;
    }
    bool ret = rmeCoreSched->Init();
    return ret;
}

void RmeSceneSched::BeginFlushAnimation()
{
    if (!FrameWindowMgr::GetInstance().GetEnable()) {
        return;
    }
    if (curWorkingStatus == 1) {
        rmeCoreSched->BeginFlushAnimation();
        RME_LOGI("[BeginFlushAnimation]:RmeSceneSched send msg success!");
    }
}

void RmeSceneSched::EndFlushAnimation()
{
    if (curWorkingStatus ==  1) {
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
        RME_LOGI("[SendCommandsStart]:RmeSceneSched send msg success!");
    }
}
} // namespace RME
} // namespace OHOS
