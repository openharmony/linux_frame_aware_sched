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

#ifndef RTG_MSG_MGR_H
#define RTG_MSG_MGR_H

#include <hilog/log.h>
#include <string>

#include "rme_log_domain.h"
#include "rtg_interface.h"
#include "single_instance.h"


namespace OHOS {
namespace RME {
class RtgMsgMgr {
    DECLARE_SINGLE_INSTANCE(RtgMsgMgr);
public:
    void Init();
    int OnForeground(const std::string appName, const int pid);
    void OnBackground(const std::string appName, const int pid, const int grpId);
    void ProcessStart(const int tid, const int grpId);
    void ProcessDied(const int pid, const int tid);
    void FpsChanged();
    void FocusChanged(const int pid, bool isFocus); // for multiwindow
};
} // namespace RME
} // namespace OHOS
#endif
