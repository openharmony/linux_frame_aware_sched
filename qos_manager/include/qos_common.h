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

constexpr int NR_QOS = 6;
constexpr int QOS_NICE_FLAG = 0x01;
constexpr int QOS_LATENCY_NICE_FLAG = 0x02;
constexpr int QOS_UCLAMP_FLAG = 0x04;
constexpr int QOS_RT_FLAG = 0x08;
constexpr int QOS_ALL_FLAG = (QOS_NICE_FLAG | QOS_LATENCY_NICE_FLAG | \
                        QOS_UCLAMP_FLAG | QOS_RT_FLAG);

constexpr int AF_RTG_ALL = 0x1fff;
constexpr int AF_RTG_DELEGATED = 0x1fff;

struct AuthCtrlData {
    int uid;
    unsigned int type;
    unsigned int rtgFlag;
    unsigned int qosFlag;
    unsigned int status;
};

struct QosCtrlData {
    int pid;
    unsigned int type;
    int level;
};

struct QosPolicyData {
    int nice;
    int latencyNice;
    int uclampMin;
    int uclampMax;
    int rtSchedPriority;
};

struct QosPolicyDatas {
    int policyType;
    unsigned int policyFlag;
    struct QosPolicyData policys[NR_QOS + 1];
};

enum AuthOperationType {
    AUTH_ENABLE = 1,
    AUTH_DELETE,
    AUTH_GET,
    AUTH_SWITCH,
    AUTH_MAX_NR,
};

enum QosOperationType {
    QOS_APPLY = 1,
    QOS_LEAVE,
    QOS_MAX_NR,
};

enum QosPolicyType {
    QOS_POLICY_DEFAULT = 1,
    QOS_POLICY_SYSTEM_SERVER = 2,
    QOS_POLICY_FRONT = 3,
    QOS_POLICY_BACK = 4,
    QOS_POLICY_MAX_NR,
};

enum AuthStatus {
    AUTH_STATUS_DISABLED = 1,
    AUTH_STATUS_SYSTEM_SERVER = 2,
    AUTH_STATUS_FOREGROUND,
    AUTH_STATUS_BACKGROUND,
    AUTH_STATUS_DEAD,
};

#define QOS_CTRL_BASIC_OPERATION \
    _IOWR(0xCC, 1, struct QosCtrlData)
#define QOS_CTRL_POLICY_OPERATION \
    _IOWR(0xCC, 2, struct QosPolicyDatas)
#define BASIC_AUTH_CTRL_OPERATION \
    _IOWR(0xCD, 1, struct AuthCtrlData)

int AuthEnable(int uid, unsigned int flag, unsigned int status);
int AuthPause(int uid);
int AuthDelete(int uid);
int QosApplyForThread(int level, int tid);
int QosLeaveForThread(int tid);
int QosPolicy(struct QosPolicyDatas *policyDatas);

} // namespace QosCommon
} // namespace OHOS
#endif // QOS_COMMON_H