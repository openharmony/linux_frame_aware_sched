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
#include <list>
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include <iosfwd>
#include "rme_constants.h"
#include "single_instance.h"
#include "app_info.h"

namespace OHOS {
namespace RME {
using namespace std;

class IntelliSenseServer {
    DECLARE_SINGLE_INSTANCE(IntelliSenseServer);
public:
    void Init();
    void ReportAppInfo(const int pid, const int uid, const std::string bundleName, ThreadState state);
    void ReportProcessInfo(const int pid, const int uid, const std::string bundleName, ThreadState state);
    void ReportCgroupChange(const int pid, const int uid, const int oldGroup, const int newGroup);
    void ReportWindowFocus(const int pid, const int uid, int isFocus);
    void ReportRenderThread(const int pid, const int uid, int renderTid);
    bool ReadXml();
    void SetPara(const int32_t currentFps, const int32_t currentRenderType);

private:
    void NewForeground(int pid);
    void NewBackground(int pid);
    void NewDiedProcess(int pid);
    void NewAppRecord(int pid);
    int TryCreateRtgForApp(AppInfo *app);
    inline CgroupPolicy CheckCgroupState(const CgroupPolicy cgroup);
    std::list<AppInfo>::iterator GetRecordOfPid(int pid);
    std::list<AppInfo> m_historyApp = {};
    std::set<std::string> m_unsupportApp = {};
    std::map<std::string, std::string> m_generalPara {};
    std::map<std::string, std::map<std::string, int>> m_subEventPara {};
    std::vector<int> m_fpsList {};
    std::vector<int> m_renderTypeList {};
    bool m_readXmlSuc = false;
    bool m_needReadXml = true;
    bool m_switch = false;
};
} // namespace RME
} // namesapce OHOS
#endif
