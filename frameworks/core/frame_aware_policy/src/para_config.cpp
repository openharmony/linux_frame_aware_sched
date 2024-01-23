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

#include "para_config.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr int FPS_MAX_VALUE = 120;
    constexpr int RENDER_TYPE_MAX_VALUE = 2;
}

DEFINE_RMELOG_INTELLISENSE("ueaServer-ParaConfig");

std::map<std::string, std::string> ParaConfig::m_generalConfig;
std::map<std::string, std::map<std::string, int>> ParaConfig::m_subEventConfig;
std::vector<int> ParaConfig::m_fpsList;
std::vector<int> ParaConfig::m_renderTypeList;

bool ParaConfig::IsXmlPrepared(const std::string& filePath)
{
    xmlDocPtr docPtr = xmlReadFile(filePath.c_str(), nullptr, XML_PARSE_NOBLANKS);
    RME_LOGI("[IsXmlPrepared]:filePath:%{public}s", filePath.c_str());
    if (docPtr == nullptr) {
        RME_LOGE("[IsXmlPrepared]:load xml error!");
        return false;
    }

    xmlNodePtr rootPtr = xmlDocGetRootElement(docPtr);
    if (rootPtr == nullptr || rootPtr->name == nullptr) {
        RME_LOGE("[IsXmlPrepared]: get root element failed!");
        xmlFreeDoc(docPtr);
        return false;
    }
    for (xmlNodePtr curNodePtr = rootPtr->xmlChildrenNode; curNodePtr != nullptr; curNodePtr = curNodePtr->next) {
        if (IsValidNode(*curNodePtr)) {
            RME_LOGE("[IsXmlPrepared]: invalid node!");
            continue;
        }
    auto nodeName = curNodePtr->name;
        if (!xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("log_open")) ||
            !xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("enable")) ||
            !xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("SOC")) ||
            !xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("frame_sched_reset_count"))) {
            ReadConfigInfo(curNodePtr);
            continue;
        }
        if (!xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("fps_list"))) {
            ReadFpsConfig(curNodePtr);
            continue;
        }
        if (!xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("render_type"))) {
            ReadRenderType(curNodePtr);
            continue;
        }
        if (!xmlStrcmp(nodeName, reinterpret_cast<const xmlChar*>("framedetect"))) {
            ReadFrameConfig(curNodePtr);
            continue;
        }
    }
    xmlFreeDoc(docPtr);
    return true;
}

bool ParaConfig::IsValidNode(const xmlNode& currNode)
{
    if (currNode.name == nullptr || currNode.type == XML_COMMENT_NODE) {
        return true;
    }
    return false;
}

void ParaConfig::ReadConfigInfo(const xmlNodePtr& root)
{
    xmlChar* context = xmlNodeGetContent(root);
    if (context == nullptr) {
        RME_LOGE("[GetConfigInfo]:read xml node error: nodeName:(%{public}s)", root->name);
        return;
    }
    std::string nodeName = reinterpret_cast<const char *>(root->name);
    m_generalConfig[nodeName] = std::string(reinterpret_cast<const char*>(context));
    xmlFree(context);
}

void ParaConfig::ReadFpsConfig(const xmlNodePtr& root)
{
    xmlChar* context = xmlNodeGetContent(root);
    if (context == nullptr) {
        RME_LOGE("[GetFpsConfig]: fps read failed!");
    return;
    }

    SplitString(std::string(reinterpret_cast<const char*>(context)), " ", m_fpsList, FPS_MAX_VALUE, "fpsList");
    xmlFree(context);
}

void ParaConfig::ReadRenderType(const xmlNodePtr& root)
{
    xmlChar* context = xmlNodeGetContent(root);
    if (context == nullptr) {
        RME_LOGE("[GetRenderType]: renderType read failed!");
        return;
    }
    SplitString(std::string(reinterpret_cast<const char*>(context)), " ",
        m_renderTypeList, RENDER_TYPE_MAX_VALUE, "renderType");
    xmlFree(context);
}

void ParaConfig::SplitString(const std::string& context, const std::string& character, std::vector<int> &mList,
    const int maxVal, const std::string& attrName)
{
    if (context == "") {
        return;
    }

    std::string toSplitStr = context + character;
    size_t pos = toSplitStr.find(character);

    while (pos != toSplitStr.npos) {
        int curVal = atoi(toSplitStr.substr(0, pos).c_str());
        if (curVal <= 0 && curVal > maxVal) {
            RME_LOGE("[SplitString]:get data error! attr name:%{public}s", attrName.c_str());
            return;
        }
        mList.push_back(curVal);
        toSplitStr = toSplitStr.substr(pos + 1, toSplitStr.size());
        pos = toSplitStr.find(character);
    }
    for (auto m : mList) {
        RME_LOGI("[SplitString]: attrName: %{public}s, list_val:%{public}d", attrName.c_str(), m);
    }
    RME_LOGI("[SplitString]:get data success!attr name:%{public}s", attrName.c_str());
}

void ParaConfig::ReadFrameConfig(const xmlNodePtr& root)
{
    // to need abnormal process!  what if the xml has problem when traversal?
    for (xmlNodePtr curNode = root->xmlChildrenNode; curNode != nullptr; curNode = curNode->next) {
        if (IsValidNode(*curNode)) {
            RME_LOGE("[IsXmlPrepared]: invalid node!");
            continue;
        }
        std::string key1 = "";
        std::string key2 = "";
        ReadAttr(curNode, "render_type", key1);
        ReadAttr(curNode, "fps_list", key2);
        std::string key = key1 + " " + key2;
        RME_LOGI("ReadFrameConfig-key:%{public}s", key.c_str());
        std::map<std::string, int> frameConfigTmp;
        xmlNodePtr curSubNode = curNode->xmlChildrenNode;
        for (; curSubNode != nullptr; curSubNode = curSubNode->next) {
            std::string nodeName = reinterpret_cast<const char*>(curSubNode->name); // char* to string
            xmlChar* context = xmlNodeGetContent(curSubNode);
            if (context == nullptr) { // if one config wrong then this config dilscard.
                RME_LOGE("[GetFrameConfig]: frame config get error! nodeName:%{public}s, key:%{public}s",
                    nodeName.c_str(), key.c_str());
                xmlFree(context);
                break;
            }
            frameConfigTmp[nodeName] = atoi(reinterpret_cast<const char*>(context));
            RME_LOGI("[GetFrameConfig]: nodeName:%{public}s, val:%{public}s",
                nodeName.c_str(), reinterpret_cast<const char*>(context));
            xmlFree(context);
        }
        m_subEventConfig[key] = frameConfigTmp;
    }
    RME_LOGI("[GetFrameConfig]:read frameconfig success!");
}

void ParaConfig::ReadAttr(xmlNodePtr& root, const std::string& attrName, std::string& res)
{
    xmlAttrPtr attrPtr = root->properties;
    while (attrPtr != nullptr) {
        if (!xmlStrcmp(attrPtr->name, reinterpret_cast<const xmlChar*>(attrName.c_str()))) {
            xmlChar* resAttr = xmlGetProp(root, reinterpret_cast<const xmlChar*>(attrName.c_str()));
            res = std::to_string(atoi((char*)resAttr));
            RME_LOGI("[ReadAttr]: attr <%{public}s> read res: %{public}s!", attrName.c_str(), res.c_str());
            xmlFree(resAttr);
        }
        attrPtr = attrPtr->next;
    }
}

std::map<std::string, std::string> ParaConfig::GetGeneralConfig()
{
    return m_generalConfig;
}

std::map<std::string, std::map<std::string, int>> ParaConfig::GetSubEventConfig()
{
    return m_subEventConfig;
}

std::vector<int> ParaConfig::GetFpsList()
{
    return m_fpsList;
}

std::vector<int> ParaConfig::GetRenderTypeList()
{
    return m_renderTypeList;
}
} // namespace RME
} // namespace OHOS
