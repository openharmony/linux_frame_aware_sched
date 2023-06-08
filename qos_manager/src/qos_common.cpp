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

constexpr unsigned int AF_QOS_DELEGATED = 0x0001;

DEFINE_RMELOG_INTELLISENSE("qos_manager");

static int TrivalOpenQosCtrlNode(void)
{
    char fileName[] = "/proc/thread-self/sched_qos_ctrl";
    int fd = open(fileName, O_RDWR);
    return fd;
}

static int TrivalOpenAuthCtrlNode(void)
{
    char fileName[] = "/dev/auth_ctrl";
    int fd = open(fileName, O_RDWR);
    return fd;
}

int AuthEnable(int uid, unsigned int flag, unsigned int status)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.uid = uid;
    data.rtgFlag = flag;
    data.qosFlag = AF_QOS_DELEGATED;
    data.status = status;
    data.type = AUTH_ENABLE;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth enable failed for uid %{public}d with status %{public}u\n", uid, status);
    }
    close(fd);
    return ret;
}

int AuthPause(int uid)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.uid = uid;
    data.rtgFlag = 0;
    data.qosFlag = AF_QOS_DELEGATED;
    data.status = AUTH_STATUS_BACKGROUND;
    data.type = AUTH_SWITCH;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth pause failed for uid %{public}d\n", uid);
    }
    close(fd);
    return ret;
}

int AuthDelete(int uid)
{
    struct AuthCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenAuthCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open auth node failed\n", gettid(), getuid());
        return fd;
    }

    data.uid = uid;
    data.type = AUTH_DELETE;

    ret = ioctl(fd, BASIC_AUTH_CTRL_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("auth delete failed for uid %{public}d\n", uid);
    }
    close(fd);
    return ret;
}

int QosApplyForThread(int level, int tid)
{
    struct QosCtrlData data;
    int fd;

    int ret;

    fd = TrivalOpenQosCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open qos node failed\n", gettid(), getuid());
        return fd;
    }

    data.level = level;
    data.type = QOS_APPLY;
    data.pid = tid;

    ret = ioctl(fd, QOS_CTRL_BASIC_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("qos apply failed for thread %{public}d\n", tid);
    }
    close(fd);
    return ret;
}

int QosLeaveForThread(int tid)
{
    struct QosCtrlData data;
    int fd;
    int ret;

    fd = TrivalOpenQosCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open qos node failed\n", gettid(), getuid());
        return fd;
    }

    data.type = QOS_LEAVE;
    data.pid = tid;

    ret = ioctl(fd, QOS_CTRL_BASIC_OPERATION, &data);
    if (ret < 0) {
        RME_LOGE("qos leave failed for thread %{public}d\n", tid);
    }
    close(fd);
    return ret;
}

int QosPolicy(struct QosPolicyDatas *policyDatas)
{
    int fd;
    int ret;

    fd = TrivalOpenQosCtrlNode();
    if (fd < 0) {
        RME_LOGE("thread %{public}d belong to user %{public}d open qos node failed\n", gettid(), getuid());
        return fd;
    }

    ret = ioctl(fd, QOS_CTRL_POLICY_OPERATION, policyDatas);
    if (ret < 0) {
        RME_LOGE("set qos policy failed for thread %{public}d\n", gettid());
    }
    close(fd);
    return ret;
}