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

#ifndef QOS_MANAGER_H
#define QOS_MANAGER_H

namespace OHOS {
namespace QOS {
enum class QosLevel {
    qos_background = 1,
    qos_utility,
    qos_default,
    qos_user_initiated,
    qos_deadline_request,
    qos_user_interactive,
};

class QosController {
public:
    static QosController& GetInstance();

    int SetThreadQosForOtherThread(enum QosLevel level, int tid);
    int ResetThreadQosForOtherThread(int tid);

private:
    QosController() = default;
    ~QosController() = default;

    QosController(const QosController&) = delete;
    QosController& operator=(const QosController&) = delete;
    QosController(QosController&&) = delete;
    QosController& operator=(const QosController&&) = delete;

    int SetPolicy();
    bool policyStatus_ = false;
};

int SetThreadQos(enum QosLevel level);
int SetQosForOtherThread(enum QosLevel level, int tid);
int ResetThreadQos();
int ResetQosForOtherThread(int tid);

} // namespace QOS
} // namespace OHOS

#endif // QOS_MANAGER_H