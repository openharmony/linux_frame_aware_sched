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

#ifndef FRAME_MSG_INTF_H
#define FRAME_MSG_INTF_H

#include "rme_constants.h"
#include "single_instance.h"
#include "event_handler.h"

namespace OHOS {
namespace RME {
class FrameMsgIntf {
public:
    static FrameMsgIntf& GetInstance();
    bool Init();
    bool GetThreadHandler();
    void ReportAppInfo(const int pid, const int uid, const std::string bundleName, ThreadState state);
    void ReportProcessInfo(const int pid, const int uid, const std::string bundleName, ThreadState state);
    void ReportCgroupChange(const int pid, const int uid, const int oldGroup, const int newGroup);
    void ReportWindowFocus(const int pid, const int uid, const int isFocus);
    void ReportRenderThread(const int pid, const int uid, const int renderTid);
    void Stop();
protected:
    FrameMsgIntf() = default;
    virtual ~FrameMsgIntf() = default;
private:
    std::mutex frameMsgIntfMutex_;
    std::shared_ptr<AppExecFwk::EventRunner> runner_;
    std::shared_ptr<AppExecFwk::EventHandler> threadHandler_;
    DISALLOW_COPY_AND_MOVE(FrameMsgIntf);
};
} // namespace RME
} // namespace OHOS
#endif
