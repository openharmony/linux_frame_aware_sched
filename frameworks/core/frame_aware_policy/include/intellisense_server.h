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

#ifndef INTELLI_SENSE_SERVER_H
#define INTELLI_SENSE_SERVER_H

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

#include "rme_constants.h"
#include "single_instance.h"
#include "event_handler.h"

namespace OHOS {
namespace RME {
using namespace std;

class IntelliSenseServer {
    DECLARE_SINGLE_INSTANCE(IntelliSenseServer);
public:
    void Init();
    void ReportMessage(std::string appName, std::string processName, int pid,
        AppStateUpdateReason reason);
    void ReportWindowFocus(const int pid, int isFocus);
    void ReportProcessInfo(const int pid, const int tid, ThreadState state);
    bool ReadXml();
    void SetPara(const int32_t currentFps, const int32_t currentRenderType);

private:
    std::map<std::string, std::string> m_generalPara {};
    std::map<std::string, std::map<std::string, int>> m_subEventPara {};
    std::vector<int> m_fpsList {};
    std::vector<int> m_renderTypeList {};
    bool m_readXmlSuc = false;
    bool m_needReadXml = true;
};
} // namespace RME
} // namesapce OHOS
#endif
