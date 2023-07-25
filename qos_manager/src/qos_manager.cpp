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

#include "qos.h"
#include <cstdlib>
#include <unistd.h>
#include "qos_common.h"
#include "rme_log_domain.h"

constexpr int ERROR_NUM = -1;

namespace OHOS {
namespace QOS {
using namespace QosCommon;
DEFINE_RMELOG_INTELLISENSE("qos_manager");

static struct QosPolicyDatas g_defaultPolicy = {
    .policyType = QOS_POLICY_DEFAULT,
    .policyFlag = QOS_ALL_FLAG,
    .policys = {
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
    }
};

static struct QosPolicyDatas g_systemServerPolicy = {
    .policyType = QOS_POLICY_SYSTEM_SERVER,
    .policyFlag = QOS_ALL_FLAG,
    .policys = {
        {0, 0, 0, 1024, 0},
        {10, 0, 0, 1024, 0},
        {5, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
    }
};

static struct QosPolicyDatas g_foregroundPolicy = {
    .policyType = QOS_POLICY_FRONT,
    .policyFlag = QOS_ALL_FLAG,
    .policys = {
        {0, 0, 0, 1024, 0},
        {10, 0, 0, 1024, 0},
        {5, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
    }
};

static struct QosPolicyDatas g_backgroundPolicy = {
    .policyType = QOS_POLICY_BACK,
    .policyFlag = QOS_ALL_FLAG & ~QOS_RT_FLAG,
    .policys = {
        {0, 0, 0, 1024, 0},
        {15, 0, 0, 1024, 0},
        {10, 0, 0, 1024, 0},
        {5, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
        {0, 0, 0, 1024, 0},
    }
};

QosController& QosController::GetInstance()
{
    static QosController instance;
    if (!instance.policyStatus_) {
        int ret = instance.SetPolicy();
        if (ret == 0) {
            instance.policyStatus_ = true;
            RME_LOGI("set qos policy success");
        }
    }
    return instance;
}

int QosController::SetPolicy()
{
    int ret;

    ret = QosPolicy(&g_defaultPolicy);
    if (ret) {
        RME_LOGE("set g_defaultPolicy failed");
        return ret;
    }

    ret = QosPolicy(&g_foregroundPolicy);
    if (ret) {
        RME_LOGE("set g_foregroundPolicy failed");
        return ret;
    }

    ret = QosPolicy(&g_backgroundPolicy);
    if (ret) {
        RME_LOGE("set g_backgroundPolicy failed");
        return ret;
    }

    ret = QosPolicy(&g_systemServerPolicy);
    if (ret) {
        RME_LOGE("set g_systemServerPolicy failed");
    }
    return ret;
}

int QosController::SetThreadQosForOtherThread(enum QosLevel level, int tid)
{
    int qos = static_cast<int>(level);
    if (level < QosLevel::qos_background || level > QosLevel::qos_user_interactive) {
        RME_LOGE("invalid qos level %{public}d", qos);
        return ERROR_NUM;
    }
    int ret = QosApplyForThread(qos, tid);
    if (ret == 0) {
        RME_LOGD("qoslevel %{public}d apply for tid %{public}d success", qos, tid);
    } else {
        RME_LOGE("qoslevel %{public}d apply for tid %{public}d failure", qos, tid);
    }

    return ret;
}

int QosController::ResetThreadQosForOtherThread(int tid)
{
    int ret = QosLeaveForThread(tid);
    if (ret == 0) {
        RME_LOGD("qoslevel reset for tid %{public}d success", tid);
    } else {
        RME_LOGE("qoslevel reset for tid %{public}d failure", tid);
    }

    return ret;
}

int SetThreadQos(enum QosLevel level)
{
    int tid = gettid();
    return QosController::GetInstance().SetThreadQosForOtherThread(level, tid);
}

int SetQosForOtherThread(enum QosLevel level, int tid)
{
    return QosController::GetInstance().SetThreadQosForOtherThread(level, tid);
}

int ResetThreadQos()
{
    int tid = gettid();
    return QosController::GetInstance().ResetThreadQosForOtherThread(tid);
}

int ResetQosForOtherThread(int tid)
{
    return QosController::GetInstance().ResetThreadQosForOtherThread(tid);
}

} // namespace QOS
} // namespace OHOS