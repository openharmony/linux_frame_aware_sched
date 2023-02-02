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
#include <hitrace_meter.h>
#include <memory>
#include <string>
#include <unistd.h>
#include "rtg_interface.h"
#include "rme_log_domain.h"

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
    StartTrace(HITRACE_TAG_ACE, "FrameS-Init");
    int ret = GetRtgEnable();
    if (ret > 0) {
        RME_LOGE("[Init]: rtgEnabled! scheme Open!ret: %{public}d", ret);
    } else {
        RME_LOGE("[Init]: do not enabled!ret: %{public}d", ret);
    }
    FinishTrace(HITRACE_TAG_ACE);
    return ret;
}

void RmeCoreSched::HandleBeginScene()
{
    HITRACE_METER(HITRACE_TAG_ACE);
}

void RmeCoreSched::BeginFlushAnimation()
{
    m_pid = getpid();
    if (m_rtg <= 0) {
        m_rtg = SearchRtgForTid(m_pid);
        if (m_rtg <= 0) {
            return;
        }
    }
    int ret = BeginFrameFreq(m_rtg, 0);
    m_uiTid = gettid();
    if (!m_uiHasSend) {
        StartTrace(HITRACE_TAG_ACE,
            ("FrameS-Begin&AddThread-rtg:" + to_string(m_rtg) + " ret:" + to_string(ret)).c_str());
        AddThreadToRtg(m_uiTid, m_rtg);
        m_uiHasSend = true;
    } else {
        StartTrace(HITRACE_TAG_ACE, "FrameS-Begin");
    }
    FinishTrace(HITRACE_TAG_ACE);
    return;
}

void RmeCoreSched::EndFlushAnimation() {}

void RmeCoreSched::BeginFlushBuild()
{
    StartTrace(HITRACE_TAG_ACE,
        ("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_BEGIN)).c_str());
    SetMargin(m_rtg, MARGIN_BEGIN);
    FinishTrace(HITRACE_TAG_ACE);
}

void RmeCoreSched::EndFlushBuild() {}

void RmeCoreSched::BeginFlushLayout() {}

void RmeCoreSched::EndFlushLayout() {}

void RmeCoreSched::BeginFlushRender()
{
    StartTrace(HITRACE_TAG_ACE,
        ("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_MIDDLE)).c_str());
    SetMargin(m_rtg, MARGIN_MIDDLE);
    FinishTrace(HITRACE_TAG_ACE);
}

void RmeCoreSched::EndFlushRender() {}

void RmeCoreSched::BeginFlushRenderFinish() {}

void RmeCoreSched::EndFlushRenderFinish() {}

void RmeCoreSched::BeginProcessPostFlush() {}

void RmeCoreSched::ProcessCommandsStart() {}

void RmeCoreSched::AnimateStart()
{
    if (m_rtg <= 0) {
        return;
    }
}

void RmeCoreSched::RenderStart()
{
    StartTrace(HITRACE_TAG_ACE,
        ("FrameS-SetMargin-rtg:" + to_string(m_rtg) + " margin:" + to_string(MARGIN_END)).c_str());
    SetMargin(m_rtg, MARGIN_END);
    FinishTrace(HITRACE_TAG_ACE);
}

void RmeCoreSched::SendCommandsStart()
{
    if (m_rtg <= 0) {
        return;
    }
    StartTrace(HITRACE_TAG_ACE, "SendCommandsStart-EndFreq");
    EndFrameFreq(m_rtg);
    FinishTrace(HITRACE_TAG_ACE);
}

void RmeCoreSched::HandleEndScene()
{
    HITRACE_METER(HITRACE_TAG_ACE);
    if (m_rtg <= 0) {
        return;
    }
    int ret = EndScene(m_rtg);
    m_uiHasSend = false;
    StartTrace(HITRACE_TAG_ACE,
        ("FrameS-EndFrameFreq-rtg:" + to_string(m_rtg) + " ret:" + to_string(ret)).c_str());
    FinishTrace(HITRACE_TAG_ACE);
}
} // namespace RME
} // OHOS
