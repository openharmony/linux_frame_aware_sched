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
#include "rme_log_domain.h"

#undef LOG_TAG
#define LOG_TAG "ueaClient-FrameUiIntf"

namespace OHOS {
namespace RME {

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
    return static_cast<int>(ErrorCode::SUCC);
}

void FrameUiIntf::BeginFlushAnimation() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_ANIMATION_START);
}

void FrameUiIntf::EndFlushAnimation() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_ANIMATION_END);
}

void FrameUiIntf::BeginFlushBuild() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_BUILD_START);
}

void FrameUiIntf::EndFlushBuild() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_BUILD_END);
}

void FrameUiIntf::BeginFlushLayout() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_LAYOUT_START);
}

void FrameUiIntf::EndFlushLayout() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_LAYOUT_END);
}

void FrameUiIntf::BeginFlushRender() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_RENDER_START);
}

void FrameUiIntf::EndFlushRender() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_RENDER_END);
}

void FrameUiIntf::BeginFlushRenderFinish() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_RENDER_FINISH_START);
}

void FrameUiIntf::EndFlushRenderFinish() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_FLUSH_RENDER_FINISH_END);
}

void FrameUiIntf::BeginProcessPostFlush() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_PROCESS_POST_FLUSH_START);
}

void FrameUiIntf::ProcessCommandsStart() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_PROCESS_COMMANDS_START);
}

void FrameUiIntf::AnimateStart() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_ANIMATE_START);
}

void FrameUiIntf::RenderStart(uint64_t timestamp) const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_RENDER_START);
}

void FrameUiIntf::RenderEnd() const
{
    return;
}

void FrameUiIntf::SendCommandsStart() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_SEND_COMMANDS_START);
}

void FrameUiIntf::BeginListFling() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_BEGIN_FRAME);
}

void FrameUiIntf::EndListFling() const
{
    if (!inited) {
        return;
    }
    FrameMsgMgr::GetInstance().EventUpdate(FrameEvent::EVENT_END_FRAME);
}

void FrameUiIntf::FlushBegin() const
{
    return;
}

void FrameUiIntf::FlushEnd() const
{
    return;
}

void FrameUiIntf::SetFrameParam(int requestId, int load, int schedFrameNum, int value) const
{
    return;
}

void FrameUiIntf::RequestNextVSync() const
{
    return;
}

void FrameUiIntf::HandleSwapBuffer() const
{
    return;
}

void FrameUiIntf::EnableSelfRender() const
{
    return;
}

void FrameUiIntf::DisableSelfRender() const
{
    return;
}

void FrameUiIntf::ReceiveVSync() const
{
    return;
}

void FrameUiIntf::MonitorGpuStart(uint32_t fenceId) const
{
    return;
}

void FrameUiIntf::MonitorGpuEnd() const
{
    return;
}

void FrameUiIntf::SendFenceId(uint32_t fenceId) const
{
    return;
}

void FrameUiIntf::ReportSchedEvent(FrameSchedEvent event, const std::unordered_map<std::string, std::string>& payload)
{
    return;
}

extern "C" void Init()
{
    FrameUiIntf::GetInstance().Init();
}

extern "C" int GetSenseSchedEnable()
{
    return FrameUiIntf::GetInstance().GetSenseSchedEnable();
}

extern "C" void BeginFlushAnimation()
{
    FrameUiIntf::GetInstance().BeginFlushAnimation();
}

extern "C" void EndFlushAnimation()
{
    FrameUiIntf::GetInstance().EndFlushAnimation();
}

extern "C" void BeginFlushBuild()
{
    FrameUiIntf::GetInstance().BeginFlushBuild();
}

extern "C" void EndFlushBuild()
{
    FrameUiIntf::GetInstance().EndFlushBuild();
}

extern "C" void BeginFlushLayout()
{
    FrameUiIntf::GetInstance().BeginFlushLayout();
}

extern "C" void EndFlushLayout()
{
    FrameUiIntf::GetInstance().EndFlushLayout();
}

extern "C" void BeginFlushRender()
{
    FrameUiIntf::GetInstance().BeginFlushRender();
}

extern "C" void EndFlushRender()
{
    FrameUiIntf::GetInstance().EndFlushRender();
}

extern "C" void BeginFlushRenderFinish()
{
    FrameUiIntf::GetInstance().BeginFlushRenderFinish();
}

extern "C" void EndFlushRenderFinish()
{
    FrameUiIntf::GetInstance().EndFlushRenderFinish();
}

extern "C" void BeginProcessPostFlush()
{
    FrameUiIntf::GetInstance().BeginProcessPostFlush();
}

extern "C" void ProcessCommandsStart()
{
    FrameUiIntf::GetInstance().ProcessCommandsStart();
}

extern "C" void AnimateStart()
{
    FrameUiIntf::GetInstance().AnimateStart();
}

extern "C" void RenderStart(uint64_t timestamp)
{
    FrameUiIntf::GetInstance().RenderStart(timestamp);
}

extern "C" void RenderEnd()
{
    FrameUiIntf::GetInstance().RenderEnd();
}

extern "C" void SendCommandsStart()
{
    FrameUiIntf::GetInstance().SendCommandsStart();
}

extern "C" void BeginListFling()
{
    FrameUiIntf::GetInstance().BeginListFling();
}

extern "C" void EndListFling()
{
    FrameUiIntf::GetInstance().EndListFling();
}

extern "C" void FlushBegin()
{
    FrameUiIntf::GetInstance().FlushBegin();
}

extern "C" void FlushEnd()
{
    FrameUiIntf::GetInstance().FlushEnd();
}

extern "C" void SetFrameParam(int requestId, int load, int schedFrameNum, int value)
{
    FrameUiIntf::GetInstance().SetFrameParam(requestId, load, schedFrameNum, value);
}

extern "C" void RequestNextVSync()
{
    FrameUiIntf::GetInstance().RequestNextVSync();
}

extern "C" void HandleSwapBuffer()
{
    FrameUiIntf::GetInstance().HandleSwapBuffer();
}

extern "C" void EnableSelfRender()
{
    FrameUiIntf::GetInstance().EnableSelfRender();
}

extern "C" void DisableSelfRender()
{
    FrameUiIntf::GetInstance().DisableSelfRender();
}

extern "C" void ReceiveVSync()
{
    FrameUiIntf::GetInstance().ReceiveVSync();
}

extern "C" void MonitorGpuStart(uint32_t fenceId)
{
    FrameUiIntf::GetInstance().MonitorGpuStart(fenceId);
}

extern "C" void MonitorGpuEnd()
{
    FrameUiIntf::GetInstance().MonitorGpuEnd();
}

extern "C" void SendFenceId(uint32_t fenceId)
{
    FrameUiIntf::GetInstance().SendFenceId(fenceId);
}

extern "C" bool IsScbScene()
{
    return false;
}

extern "C" void ReportSchedEvent(FrameSchedEvent event, const std::unordered_map<std::string, std::string>& payload)
{
    FrameUiIntf::GetInstance().ReportSchedEvent(event, payload);
}
} // namespace RME
} // namespace OHOS
