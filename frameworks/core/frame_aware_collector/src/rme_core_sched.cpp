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
    constexpr int CPU_MARGIN_THREE = -3;
    constexpr int CPU_MARGIN_FIVE = -5;
    constexpr int CPU_MARGIN_SEVEN = -7;
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
    int ret = GetRtgEnable();
    if (ret > 0) {
        RME_LOGE("[Init]: rtgEnabled! scheme Open!ret: %{public}d", ret);
    } else {
        RME_LOGE("[Init]: do not enabled!ret: %{public}d", ret);
    }
    return ret;
}

void RmeCoreSched::BeginFlushAnimation()
{
    m_currentPid = getpid();
    int tid = gettid();
    if (m_currentRtg == -1) {
        m_currentRtg = SearchRtgForTid(m_currentPid);
        if (m_currentRtg <= 0) {
            RME_LOGE("[BeginFlushAnimation]:Search rtg failed!pid %{public}d,Rtg: %{public}d",
                m_currentPid, m_currentRtg);
            return;
        } else {
            RME_LOGI("[BeginFlushAnimation]:Search rtg sucess Rtg, val:%{public}d", m_currentRtg);
        }
    }
    int ret = BeginFrameFreq(m_currentRtg, 0);
    RME_LOGI("[BeginFlushAnimation]:BeginFrameFreq,Rtg:%{public}d,Pid:%{public}d,tid:%{public}d, ret:%{public}d!",
        m_currentRtg, m_currentPid, tid, ret);
    return;
}

void RmeCoreSched::EndFlushAnimation() {}

void RmeCoreSched::BeginFlushBuild()
{
    SetMargin(m_currentRtg, CPU_MARGIN_THREE);
}

void RmeCoreSched::EndFlushBuild() {}

void RmeCoreSched::BeginFlushLayout() {}

void RmeCoreSched::EndFlushLayout() {}

void RmeCoreSched::BeginFlushRender()
{
    SetMargin(m_currentRtg, CPU_MARGIN_FIVE);
}

void RmeCoreSched::EndFlushRender() {}

void RmeCoreSched::BeginFlushRenderFinish()
{
    SetMargin(m_currentRtg, CPU_MARGIN_SEVEN);
}

void RmeCoreSched::EndFlushRenderFinish() {}

void RmeCoreSched::BeginProcessPostFlush() {}

void RmeCoreSched::ProcessCommandsStart() {}

void RmeCoreSched::AnimateStart()
{
    if (isRenderAdd) {
        return;
    }
    if (m_currentRtg <= 0) {
        RME_LOGE("[AnimateStart]: search rtg error! Rtg:%{public}d, Pid:%{public}d", m_currentRtg, m_currentPid);
        return;
    }
    int rendertid = gettid();
    int ret = AddThreadToRtg(rendertid, m_currentRtg);
    if (ret) {  // 1 means false.
        RME_LOGE("[AnimateStart]:add rtg failed! rtGrp: %{public}d, rendertid: %{public}d, m_currentPid:%{public}d",
            m_currentRtg, rendertid, m_currentPid);
    } else {
        isRenderAdd = true;
        RME_LOGI("[AnimateStart]:add rtg SUCESS! rendertid:%{public}d, rtGrp:%{public}d", rendertid, m_currentRtg);
    }
}

void RmeCoreSched::RenderStart()
{
    SetMargin(m_currentRtg, CPU_MARGIN_THREE);
}

void RmeCoreSched::SendCommandsStart()
{
    int pid = getpid();
    if (m_currentRtg <= 0) {
        RME_LOGE("[SendCommandStart]: m_currentRtg error! rtGrp:%{public}d, m_currentPid:%{public}d, pid:%{public}d!",
            m_currentRtg, m_currentPid, pid);
        return;
    }
    int ret = EndFrameFreq(m_currentRtg);
    RME_LOGI("[SendCommandsStart]: set EndFrameFreq, ret: %{public}d, m_currentPid:%{publid}d, pid:%{public}d!",
        ret, m_currentPid, pid);
}
} // namespace RME
} // OHOS
