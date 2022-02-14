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

#include "frame_ui_intf.h"

#include "frame_info_const.h"
#include "rme_constants.h"
#include "frame_msg_mgr.h"

namespace OHOS {
namespace RME {

DEFINE_RMELOG_INTELLISENSE("ueaCleint-FrameUiIntf");

//TO DO: need to check the init logic.

FrameUiIntf& FrameUiIntf::GetInstance()
{
    static FrameUiIntf instance;
    return instance;
}

void FrameUiIntf::Init()
{
    if (inited) {
        return;
    }
    int ret = FrameMsgMgr::GetInstance().Init();
    if (!ret) {
        inited = false;
    } else {
        inited = true;
    }
    RME_LOGI("[Init]:ret:%{public}d, inited:%{public}d", ret, inited);
}

int FrameUiIntf::GetSenseSchedEnable()
{
    if (!inited) {
        return static_cast<int>(ErrorCode::FAIL);
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_RTG_ENABLE, EventState::ENABLE);
    return static_cast<int>(ErrorCode::SUCC);
}

void FrameUiIntf::BeginFlushAnimation()
{

    RME_LOGI("[BeginFlushAnimation]:start");
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_ANIMATION, EventState::EVENT_ENTER);
}

void FrameUiIntf::EndFlushAnimation()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_BUILD, EventState::EVENT_EXIT);
}

void FrameUiIntf::BeginFlushBuild()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_BUILD, EventState::EVENT_ENTER);
    RME_LOGI("[BeginFlushBuild]:success"); 
}

void FrameUiIntf::EndFlushBuild()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_BUILD, EventState::EVENT_EXIT);
}

void FrameUiIntf::BeginFlushLayout()
{ 
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_LAYOUT, EventState::EVENT_ENTER);
}

void FrameUiIntf::EndFlushLayout()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_LAYOUT, EventState::EVENT_EXIT);
}

void FrameUiIntf::BeginFlushRender()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_RENDER, EventState::EVENT_ENTER);
}

void FrameUiIntf::EndFlushRender()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::FLUSH_RENDER, EventState::EVENT_EXIT);
}

void FrameUiIntf::BeginProcessPostFlush()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::PROCESS_POST_FLUSH, EventState::EVENT_ENTER);
}

void FrameUiIntf::ProcessCommandsStart()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::PROCESS_COMMANDS, EventState::EVENT_ENTER);
}

void FrameUiIntf::AnimateStart()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::ANIMATE, EventState::EVENT_ENTER);
}

void FrameUiIntf::RenderStart()
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::RENDER, EventState::EVENT_ENTER);
}

void FrameUiIntf::SendCommandsStart()
{
    RME_LOGI("[SendCommandsStart]:start");
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::SEND_COMMANDS, EventState::EVENT_ENTER);
    RME_LOGI("[SendCommandsStart]:success");
}

} // namespace RME
} // namespace OHOS
