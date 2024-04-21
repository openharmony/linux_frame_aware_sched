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

struct TraceHandle* CreateTraceTag([[maybe_unused]] const std::string traceTag)
{
    return nullptr;
}

void SetTraceLimit([[maybe_unused]] struct TraceHandle* traceHandle, [[maybe_unused]] unsigned int traceLimit)
{
    return;
}

void EnableTraceForThread([[maybe_unused]] struct TraceHandle* traceHandle)
{
    return;
}

void StartFrameTrace([[maybe_unused]] struct TraceHandle* traceHandle)
{
    return;
}

void StopFrameTrace([[maybe_unused]] struct TraceHandle* traceHandle)
{
    return;
}

void TraceAndExecute([[maybe_unused]] std::function<void()> &&func, [[maybe_unused]] TraceType traceType)
{
    return;
}

bool JudgeUnequalFrameTrace([[maybe_unused]] struct TidHandle* tidHandle)
{
    return false;
}

void UpdateFrameTraceTid([[maybe_unused]] struct TidHandle* tidHandle)
{
    return;
}

bool FrameAwareTraceEnable([[maybe_unused]] const std::string &traceTag)
{
    return false;
}

void QuickStartFrameTrace([[maybe_unused]] const std::string &traceTag)
{
    return;
}

void QuickEndFrameTrace([[maybe_unused]] const std::string &traceTag)
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
