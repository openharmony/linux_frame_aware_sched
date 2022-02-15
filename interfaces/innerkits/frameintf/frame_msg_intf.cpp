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

#include "frame_msg_intf.h"
#include "intellisense_server.h"
#include "event_handler.h"
#include "rme_log_domain.h"

namespace OHOS {
namespace RME {

DEFINE_RMELOG_INTELLISENSE("ueaServer-FrameMsgIntf");

FrameMsgIntf& FrameMsgIntf::GetInstance()
{
    static FrameMsgIntf instance;
    return instance;
}

bool FrameMsgIntf::Init()
{
    RME_LOGI("init begin!");
    if (!GetThreadHandler()) {
        return false;
    }
    threadHandler_->PostTask([] {
        IntelliSenseServer::GetInstance().Init();
    });
    return true;
}

bool FrameMsgIntf::GetThreadHandler()
{
    if (threadHandler_ == nullptr) {
        threadHandler_ = std::make_shared<AppExecFwk::EventHandler>(AppExecFwk::EventRunner::Create());
        if (threadHandler_ == nullptr) {
            RME_LOGI("failed to create thread handler!");
            return false;
        }
    }
    RME_LOGI("Init process success!");
    return true;
}


void FrameMsgIntf::ReportAppInfo(std::string appName, std::string processName, int pid, AppStateUpdateReason reason)
{
    if (threadHandler_ == nullptr) {
        RME_LOGI("[ReportAppInfo]:threandHandler none!");
        return;
    }
    threadHandler_->PostTask([appName, processName, pid, reason] {
        IntelliSenseServer::GetInstance().ReportMessage(appName, processName, pid, reason);
    });
}

void FrameMsgIntf::ReportWindowFocus(const int pid, const int isFocus)
{
    if (threadHandler_ == nullptr) {
        RME_LOGI("[ReportWindowFocus]:threandHandler none!");
        return;
    }
    threadHandler_->PostTask([pid, isFocus] {
        IntelliSenseServer::GetInstance().ReportWindowFocus(pid, isFocus);
    });
}

void FrameMsgIntf::ReportProcessInfo(const int pid, const int tid, ThreadState state)
{

    if (threadHandler_ == nullptr) {
        RME_LOGI("[ReportWindowFocus]:threandHandler none!");
        return;
    }
    threadHandler_->PostTask([pid, tid, state] {
        IntelliSenseServer::GetInstance().ReportProcessInfo(pid, tid, state);
    });
}

void FrameMsgIntf::Stop()
{
    if (threadHandler_ != nullptr) {
        threadHandler_->RemoveAllEvents();
        threadHandler_ = nullptr;
    }
    RME_LOGI("stop eventRunner success!");
}

} // namespace RME
} // namespace OHOS

