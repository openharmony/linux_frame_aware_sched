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

#include "rme_scoped_trace.h"

#include <hitrace_meter.h>
#include <securec.h>

namespace OHOS {
namespace RME {
namespace {
const size_t MAX_STRING_SIZE = 128;
}

bool RmeTraceEnable()
{
    return true;
}

void RmeTraceBegin(const char* name)
{
    if (name == nullptr) {
        return;
    }
    std::string nameStr(name);
    StartTrace(HITRACE_TAG_ACE, nameStr);
}

void RmeTraceEnd()
{
    if (RmeTraceEnable()) {
        FinishTrace(HITRACE_TAG_ACE);
    }
}

bool RmeTraceBeginWithArgv(const char* format, va_list args)
{
    char name[MAX_STRING_SIZE] = { 0 };
    if (vsnprintf_s(name, sizeof(name), sizeof(name) - 1, format, args) < 0) {
        return false;
    }
    RmeTraceBegin(name);
    return true;
}

bool RmeTraceBeginWithArgs(const char* format, ...)
{
    if (RmeTraceEnable()) {
        va_list args;
        va_start(args, format);
        bool retVal = RmeTraceBeginWithArgv(format, args);
        va_end(args);
        return retVal;
    }
    return false;
}

RmeScopedTrace::RmeScopedTrace(const char* format, ...) : traceEnabled_(RmeTraceEnable())
{
    if (traceEnabled_) {
        va_list args;
        va_start(args, format);
        traceEnabled_ = RmeTraceBeginWithArgv(format, args);
        va_end(args);
    }
}

RmeScopedTrace::~RmeScopedTrace()
{
    if (traceEnabled_) {
        RmeTraceEnd();
    }
}
} // namespace RME
} // namespace OHOS
