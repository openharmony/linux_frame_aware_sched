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
#include "qos_common.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "rme_log_domain.h"

#undef LOG_TAG
#define LOG_TAG "qos_manager"

constexpr unsigned int AF_QOS_DELEGATED = 0x0001;

namespace OHOS {
namespace QosCommon {

static int TrivalOpenAuthCtrlNode(void)
{
    char fileName[] = "/dev/auth_ctrl";
    int fd = open(fileName, O_RDWR);
    return fd;
}

int AuthEnable(int pid, unsigned int flag, unsigned int status)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.pid = pid;
    data.rtgFlag = flag;
    data.qosFlag = AF_QOS_DELEGATED;
    data.status = status;
    data.type = AUTH_ENABLE;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth enable failed for pid %{public}d with status %{public}u\n", pid, status);
    }
    close(fd);
    return ret;
}

int AuthPause(int pid)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.pid = pid;
    data.rtgFlag = 0;
    data.qosFlag = AF_QOS_DELEGATED;
    data.status = AUTH_STATUS_BACKGROUND;
    data.type = AUTH_SWITCH;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth pause failed for pid %{public}d\n", pid);
    }
    close(fd);
    return ret;
}

int AuthDelete(int pid)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.pid = pid;
    data.type = AUTH_DELETE;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth delete failed for pid %{public}d\n", pid);
    }
    close(fd);
    return ret;
}

} // namespace QosCommon
} // namespace OHOS