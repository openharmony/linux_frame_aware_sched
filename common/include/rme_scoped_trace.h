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

#ifndef RME_SCOPED_TRACE_H
#define RME_SCOPED_TRACE_H

#include "hitrace_meter.h"

#include <cstdarg>
#include <cstdio>

#define RME_SCOPED_TRACE(fmt, ...) RmeScopedTrace rmeScopedTrace(fmt, ##__VA_ARGS__)
#ifdef RME_DEBUG
#define RME_DEBUG_SCOPED_TRACE(fmt, ...) RmeScopedTrace rmeScopedTrace(fmt, ##__VA_ARGS__)
#else
#define RME_DEBUG_SCOPED_TRACE(fmt, ...)
#endif

#define RME_FUNCTION_TRACE() RME_SCOPED_TRACE(__func__)

namespace OHOS {
namespace RME {
bool RmeTraceEnable();
void RmeTraceBegin(const char* name);
bool RmeTraceBeginWithArgs(const char* format, ...) __attribute__((__format__(printf, 1, 2)));
bool RmeTraceBeginWithArgvs(const char* format, va_list args);
void RmeTraceEnd();

class RmeScopedTrace final {
public:
    explicit RmeScopedTrace(const char* format, ...) __attribute__((__format__(printf, 2, 3)));
    ~RmeScopedTrace();

private:
    bool traceEnabled_ { false };
};
} // namespace RME
} // namespace OHOS
#endif
