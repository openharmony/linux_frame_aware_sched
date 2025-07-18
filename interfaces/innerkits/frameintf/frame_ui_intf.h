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

#ifndef FRAME_UI_INTF_H
#define FRAME_UI_INTF_H

#include <cstdint>
#include "frame_info_const.h"
#include <unordered_map>

namespace OHOS {
namespace RME {
class FrameUiIntf {
public:
    static FrameUiIntf& GetInstance();
    void Init();
    int GetSenseSchedEnable();
    void ReportSchedEvent(FrameSchedEvent event, const std::unordered_map<std::string, std::string>& payload);
    void BeginFlushAnimation() const;
    void EndFlushAnimation() const;

    void BeginFlushBuild() const;
    void EndFlushBuild() const;
    void BeginFlushLayout() const;
    void EndFlushLayout() const;

    void BeginFlushRender() const;
    void EndFlushRender() const;

    void BeginFlushRenderFinish() const;
    void EndFlushRenderFinish() const;

    void BeginProcessPostFlush() const;
    void ProcessCommandsStart() const;
    void AnimateStart() const;
    void RenderStart(uint64_t timestamp) const;
    void RenderEnd() const;
    void SendCommandsStart() const;

    void BeginListFling() const;
    void EndListFling() const;
    void FlushBegin() const;
    void FlushEnd() const;
    void SetFrameParam(int requestId, int load, int schedFrameNum, int value) const;

    void HandleSwapBuffer() const;
    void RequestNextVSync() const;
    void EnableSelfRender() const;
    void DisableSelfRender() const;
    void ReceiveVSync() const;
    void MonitorGpuStart(uint32_t fenceId) const;
    void MonitorGpuEnd() const;
    void SendFenceId(uint32_t fenceId) const;
private:
    bool inited = false;
};
} // namespace RME
} // namespace OHOS
#endif
