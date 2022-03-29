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

#ifndef FRAME_INFO_CONST_H
#define FRAME_INFO_CONST_H

namespace OHOS {
namespace RME {
enum class FrameEvent {
    EVENT_BEGIN_FRAME,
    EVENT_FLUSH_ANIMATION_START,
    EVENT_FLUSH_ANIMATION_END,
    EVENT_FLUSH_BUILD_START,
    EVENT_FLUSH_BUILD_END,
    EVENT_FLUSH_LAYOUT_START,
    EVENT_FLUSH_LAYOUT_END,
    EVENT_FLUSH_RENDER_START,
    EVENT_FLUSH_RENDER_END,
    EVENT_FLUSH_RENDER_FINISH_START,
    EVENT_FLUSH_RENDER_FINISH_END,
    EVENT_PROCESS_POST_FLUSH_START,
    EVENT_PROCESS_COMMANDS_START,
    EVENT_ANIMATE_START,
    EVENT_RENDER_START,
    EVENT_SEND_COMMANDS_START,
    EVENT_END_FRAME,
    EVENT_SET_PARAM,
    UNKNOWN,
};

enum class SceneEvent {
    SCENE_INVALID = 0,
    VIDEO,
    SLIDE,
    GAME,
    CLICK,
    SCENE_MAX,
};
} // namespace RME
} // namespace OHOS

#endif
