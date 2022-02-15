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

DEFINE_RMELOG_INTELLISENSE("ueaClient-RmeCoreSched");

RmeCoreSched::RmeCoreSched()
{
    return;
}

RmeCoreSched::~RmeCoreSched()
{
    return;
}

void RmeCoreSched::Init()
{
    int ret = EnableRtg(true);
    if (ret < 0) {
        RME_LOGE("[Init]: enableRtg failed!");
    }
    return;
}

void RmeCoreSched::BeginFlushAnimation()
{
    int tid = gettid();
    int rtGrp = SearchRtgForTid(tid);
    if (rtGrp < 0) {
        RME_LOGE("[BeginFlushAnimation]: search rtg for tid failed!");
    }
    int ret = BeginFrameFreq(rtGrp, 0);
    RME_LOGI("[BeginFlushAnimation]: set BeginFrameFreq(2, 0), ret: %{public}d!",ret);
    return;
}

void RmeCoreSched::EndFlushAnimation()
{
}

void RmeCoreSched::BeginFlushBuild()
{
}

void RmeCoreSched::EndFlushBuild()
{
}

void RmeCoreSched::BeginFlushLayout()
{
}

void RmeCoreSched::EndFlushLayout()
{
}

void RmeCoreSched::BeginFlushRender()
{
}

void RmeCoreSched::EndFlushRender()
{
}

void RmeCoreSched::BeginProcessPostFlush()
{
}

void RmeCoreSched::ProcessCommandsStart()
{
}

void RmeCoreSched::AnimateStart()
{
    int rendertid = gettid();
    int uitid = getpid(); // ui tid equals pid now.
    int rtGrp = SearchRtgForTid(uitid);
    if (rtGrp < 0) {
        RME_LOGE("[AnimateStart]: search rtg for tid failed! uitid:%{public}d", uitid);
    }
    int ret = AddThreadToRtg(rendertid, rtGrp);
    RME_LOGE("[AnimateStart]:add rtg grp failed! ret: %{public}d, rendertid: \
        %{public}d, rtGrp: %{public}d", ret, rendertid, rtGrp);
}

void RmeCoreSched::RenderStart()
{
}

void RmeCoreSched::SendCommandsStart()
{
    int tid = gettid();
    int rtGrp = SearchRtgForTid(tid);
    if (rtGrp < 0) {
        RME_LOGE("[BeginFlushAnimation]: search rtg for tid failed!");
    }
    int ret = EndFrameFreq(rtGrp, 16); 
    RME_LOGI("[SendCommandsStart]: set EndFrameFreq(2, 16), ret: %{public}d!", ret);
}

} // namespace RME
} // OHOS
