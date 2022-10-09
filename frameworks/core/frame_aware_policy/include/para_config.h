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

#ifndef PARA_CONFIG_H
#define PARA_CONFIG_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cstdlib>
#include "third_party/libxml2/include/libxml/parser.h"
#include "third_party/libxml2/include/libxml/tree.h"
#include "third_party/libxml2/include/libxml/xpath.h"
#include "rme_log_domain.h"
#include "single_instance.h"

namespace OHOS {
namespace RME {
class ParaConfig {
public:
    ParaConfig();
    ~ParaConfig();

    ParaConfig(const ParaConfig&) = delete;
    ParaConfig& operator=(const ParaConfig&) = delete;
    ParaConfig(ParaConfig&&) = delete;
    ParaConfig& operator=(const ParaConfig&&) = delete;

    static bool IsXmlPrepared(const std::string& filePath); // input  parameters need gaurantee
    static std::map<std::string, std::string> GetGeneralConfig();
    static std::map<std::string, std::map<std::string, int>> GetSubEventConfig();
    static std::vector<int> GetFpsList();
    static std::vector<int> GetRenderTypeList();
private:
    static std::map<std::string, std::string> m_generalConfig;
    static std::map<std::string, std::map<std::string, int>> m_subEventConfig;
    static std::vector<int> m_fpsList;
    static std::vector<int> m_renderTypeList;

    static void ReadAttr(xmlNodePtr& root, const std::string& attrName, std::string& res);
    static void ReadFrameConfig(const xmlNodePtr& root);
    static void ReadConfigInfo(const xmlNodePtr& root);
    static void ReadFpsConfig(const xmlNodePtr& root);
    static void ReadRenderType(const xmlNodePtr& root);
    static void SplitString(const std::string& context, const std::string& character,
        std::vector<int> &mList, const int maxVal, const std::string& attrName);
    static bool IsValidNode(const xmlNode& currNode);
};
} // namespace RME
} // namespace OHOS
#endif
