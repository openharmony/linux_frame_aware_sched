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
enum class FrameSchedEvent {
    SCHED_EVENT_BASE = 0,
    INIT = 1,
    RS_RENDER_START = 10001,
    RS_RENDER_END = 10002,
    RS_UNI_RENDER_START = 10003,
    RS_UNI_RENDER_END = 10004,
    RS_HARDWARE_START = 10005,
    RS_HARDWARE_END = 10006,
    RS_HARDWARE_INFO = 10007,
    RS_BUFFER_COUNT = 10008,
    RS_FRAME_DEADLINE = 10009,
    RS_UNBLOCK_MAINTHREAD = 10010,
    RS_POST_AND_WAIT = 10011,
    RS_BEGIN_FLUSH = 10012,
    RS_BLUR_PREDICT = 10013,
    RS_UNMARSHAL_DATA = 10014,
    RS_RECEIVE_VSYNC = 10015,
    RS_SWAP_BUFFER = 10016,
    RS_DDGR_TASK = 10017,
    RS_COMPOSER_INFO = 10018,
    RS_ADD_SCREENID = 10019,
    RS_DEL_SCREENID = 10020,
    RS_REQUEST_NEXT_VSYNC = 10021,
    RS_SEND_COMMANDS_START = 10022,
    UI_FLUSH_BEGIN = 20001,
    UI_FLUSH_END = 20002,
    UI_SCB_WORKER_BEGIN = 20003,
    UI_SCB_WORKER_END = 20004,
    UI_EVENT_HANDLE_BEGIN = 20005,
    UI_CHECKPOINT = 20006,
    SET_FRAME_PARAM = 30001,
    GPU_SCB_SCENE_INFO = 40001,
    GPU_SCB_FENCE_ID = 40002,
    GPU_SCB_FREQUENCY_START = 40003,
    GPU_SCB_FREQUENCY_END = 40004,
    SCHED_EVENT_MAX,
};

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
