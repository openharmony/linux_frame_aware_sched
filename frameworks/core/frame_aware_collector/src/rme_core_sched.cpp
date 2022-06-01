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

#include "rme_core_sched.h"
#include "rtg_interface.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr int MARGIN_BEGIN = -3;
    constexpr int MARGIN_MIDDLE = -5;
    constexpr int MARGIN_END = -13;
}
DEFINE_RMELOG_INTELLISENSE("ueaClient-RmeCoreSched");
RmeCoreSched::RmeCoreSched() {}

RmeCoreSched::~RmeCoreSched()
{
    EnableRtg(false);
    return;
}

bool RmeCoreSched::Init()
{
    RmeTraceBegin("FrameS-Init");
    int ret = GetRtgEnable();
    if (ret > 0) {
        RME_LOGE("[Init]: rtgEnabled! scheme Open!ret: %{public}d", ret);
    } else {
        RME_LOGE("[Init]: do not enabled!ret: %{public}d", ret);
    }
    RmeTraceEnd();
    return ret;
}

void RmeCoreSched::HandleBeginScene()
{
    RME_FUNCTION_TRACE();
}

void RmeCoreSched::BeginFlushAnimation()
{
    m_pid = getpid();
    if (m_rtg <= 0) {
        m_rtg = SearchRtgForTid(m_pid);
        if (m_rtg <= 0) {
            RME_LOGW("[BeginFlushAnimation]:Search rtg empty!pid %{public}d,Rtg: %{public}d",
                m_pid, m_rtg);
            return;
        } else {
            RME_LOGI("[BeginFlushAnimation]:Search rtg sucess Rtg, val:%{public}d", m_rtg);
        }
    }
    int ret = BeginFrameFreq(m_rtg, 0);
    RmeTraceBegin(("FrameS-Begin&AddThread-rtg:" + to_string(m_rtg) + " ret:" + to_string(ret)).c_str());
    m_uiTid = gettid();
    if (!m_uiHasSend) {
        AddThreadToRtg(m_uiTid, m_rtg);
        m_uiHasSend = true;
    }
    RmeTraceEnd();
    return;
}

void RmeCoreSched::EndFlushAnimation() {}

void RmeCoreSched::BeginFlushBuild()
{
    RmeTraceBegin(("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_BEGIN)).c_str());
    SetMargin(m_rtg, MARGIN_BEGIN);
    RmeTraceEnd();
}

void RmeCoreSched::EndFlushBuild() {}

void RmeCoreSched::BeginFlushLayout() {}

void RmeCoreSched::EndFlushLayout() {}

void RmeCoreSched::BeginFlushRender()
{
    RmeTraceBegin(("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_MIDDLE)).c_str());
    SetMargin(m_rtg, MARGIN_MIDDLE);
    RmeTraceEnd();
}

void RmeCoreSched::EndFlushRender() {}

void RmeCoreSched::BeginFlushRenderFinish() {}

void RmeCoreSched::EndFlushRenderFinish() {}

void RmeCoreSched::BeginProcessPostFlush() {}

void RmeCoreSched::ProcessCommandsStart() {}

void RmeCoreSched::AnimateStart()
{
    if (m_rtg <= 0) {
        RME_LOGW("[AnimateStart]: search rtg empty! Rtg:%{public}d, Pid:%{public}d", m_rtg, m_pid);
        return;
    }
    if (!m_renderHasSend) {
        m_renderTid = gettid();
        int ret = AddThreadToRtg(m_renderTid, m_rtg);
        RmeTraceBegin(("FrameS-Add rtg:" + to_string(m_rtg) + " ret:" + to_string(ret)).c_str());
        m_renderHasSend = true;
        RmeTraceEnd();
    }
}

void RmeCoreSched::RenderStart()
{
    RmeTraceBegin(("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_END)).c_str());
    SetMargin(m_rtg, MARGIN_END);
    RmeTraceEnd();
}

void RmeCoreSched::SendCommandsStart()
{
    if (m_rtg <= 0) {
        return;
    }
    RmeTraceBegin("SendCommandsStart-EndFreq");
    EndFrameFreq(m_rtg);
    RmeTraceEnd();
}

void RmeCoreSched::HandleEndScene()
{
    RME_FUNCTION_TRACE();
    if (m_rtg <= 0) {
        RME_LOGW("[HandleEndScene]: search rtg empty! rtGrp:%{public}d, m_pid:%{public}d!",
            m_rtg, m_pid);
        return;
    }
    int ret = EndScene(m_rtg);
    m_renderHasSend = false;
    m_uiHasSend = false;
    RmeTraceBegin(("FrameS-EndFrameFreq-rtg:" + to_string(m_rtg) + " ret:" + to_string(ret)).c_str());
    RmeTraceEnd();
}
} // namespace RME
} // OHOS
