/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#ifndef QOS_COMMON_H
#define QOS_COMMON_H

namespace OHOS {
namespace QosCommon {

constexpr int AF_RTG_ALL = 0x1fff;

struct AuthCtrlData {
    int pid;
    unsigned int type;
    unsigned int rtgFlag;
    unsigned int qosFlag;
    unsigned int status;
};

enum AuthOperationType {
    AUTH_ENABLE = 1,
    AUTH_DELETE,
    AUTH_GET,
    AUTH_SWITCH,
    AUTH_MAX_NR,
};

enum AuthStatus {
    AUTH_STATUS_DISABLED = 1,
    AUTH_STATUS_SYSTEM_SERVER = 2,
    AUTH_STATUS_FOREGROUND,
    AUTH_STATUS_BACKGROUND,
    AUTH_STATUS_DEAD,
};

#define BASIC_AUTH_CTRL_OPERATION \
    _IOWR(0xCD, 1, struct AuthCtrlData)

int AuthEnable(int pid, unsigned int flag, unsigned int status);
int AuthPause(int pid);
int AuthDelete(int pid);

} // namespace QosCommon
} // namespace OHOS
#endif // QOS_COMMON_H