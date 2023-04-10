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

#ifndef FRAME_TRACE_HPP
#define FRAME_TRACE_HPP

#include <functional>
#include <string>

namespace FRAME_TRACE {

struct TraceHandle;
struct TidHandle;

enum class TraceType {
    QUICK_TRACE,
    SLOW_TRACE
};

struct TraceHandle* CreateTraceTag(const std::string traceTag);
void SetTraceLimit(struct TraceHandle* traceHandle, unsigned int traceLimit);
void EnableTraceForThread(struct TraceHandle* traceHandle);
void StartFrameTrace(struct TraceHandle* traceHandle);
void StopFrameTrace(struct TraceHandle* traceHandle);
void TraceAndExecute(std::function<void()> &&func, TraceType traceType);
bool JudgeUnequalFrameTrace(struct TidHandle* tidHandle);
void UpdateFrameTraceTid(struct TidHandle* tidHandle);
bool FrameAwareTraceEnable(const std::string &traceTag);
void QuickStartFrameTrace(const std::string &traceTag);
void QuickEndFrameTrace(const std::string &traceTag);
bool FrameAwareTraceIsOpen();
bool FrameAwareTraceOpen();
bool FrameAwareTraceClose();
bool IsEnabled();
}

#endif
