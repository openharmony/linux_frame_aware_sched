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

#include "frame_trace.h"

namespace FRAME_TRACE {

struct TraceHandle* CreateTraceTag(const std::string traceTag)
{
    return nullptr;
}

void SetTraceLimit(struct TraceHandle* traceHandle, unsigned int traceLimit)
{
    return;
}

void EnableTraceForThread(struct TraceHandle* traceHandle)
{
    return;
}

void StartFrameTrace(struct TraceHandle* traceHandle)
{
    return;
}

void StopFrameTrace(struct TraceHandle* traceHandle)
{
    return;
}

void TraceAndExecute(std::function<void()> &&func, TraceType traceType)
{
    return;
}

bool JudgeUnequalFrameTrace(struct TidHandle* tidHandle)
{
    return false;
}

void UpdateFrameTraceTid(struct TidHandle* tidHandle)
{
    return;
}

bool FrameAwareTraceEnable(const std::string &traceTag)
{
    return false;
}

void QuickStartFrameTrace(const std::string &traceTag)
{
    return;
}

void QuickEndFrameTrace(const std::string &traceTag)
{
    return;
}

bool FrameAwareTraceIsOpen()
{
    return false;
}

bool FrameAwareTraceOpen()
{
    return false;
}

bool FrameAwareTraceClose()
{
    return false;
}

bool IsEnabled()
{
    return false;
}
}
