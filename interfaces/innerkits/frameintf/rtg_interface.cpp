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

#include "rtg_interface.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <securec.h>
#include <cstdio>
#include <string>
#include <vector>
#include <cerrno>
#include "bits/ioctl.h"
#include "rme_log_domain.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr size_t MAX_LENGTH = 100;
    constexpr size_t MAX_STR_LEN = 100;
    constexpr int RTG_TYPE_MAX = 3;
}
DEFINE_RMELOG_INTELLISENSE("rtg_interface");

const char RTG_SCHED_IPC_MAGIC = 0xAB;
static int g_fd = -1;

#define CMD_ID_SET_ENABLE \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_ENABLE, struct rtg_enable_data)
#define CMD_ID_SET_RTG \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_RTG, struct rtg_str_data)
#define CMD_ID_SET_CONFIG \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_CONFIG, struct rtg_str_data)
#define CMD_ID_SET_RTG_ATTR \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_RTG_ATTR, struct rtg_str_data)
#define CMD_ID_BEGIN_FRAME_FREQ \
    _IOWR(RTG_SCHED_IPC_MAGIC, BEGIN_FRAME_FREQ, struct proc_state_data)
#define CMD_ID_END_FRAME_FREQ \
    _IOWR(RTG_SCHED_IPC_MAGIC, END_FRAME_FREQ, struct proc_state_data)
#define CMD_ID_END_SCENE \
    _IOWR(RTG_SCHED_IPC_MAGIC, END_SCENE, struct proc_state_data)
#define CMD_ID_SET_MIN_UTIL \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_MIN_UTIL, struct proc_state_data)
#define CMD_ID_SET_MAX_UTIL \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_MAX_UTIL, struct proc_state_data)
#define CMD_ID_SET_MARGIN \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_MARGIN, struct proc_state_data)
#define CMD_ID_LIST_RTG \
    _IOWR(RTG_SCHED_IPC_MAGIC, LIST_RTG, struct rtg_info)
#define CMD_ID_LIST_RTG_THREAD \
    _IOWR(RTG_SCHED_IPC_MAGIC, LIST_RTG_THREAD, struct rtg_grp_data)
#define CMD_ID_SEARCH_RTG \
    _IOWR(RTG_SCHED_IPC_MAGIC, SEARCH_RTG, struct proc_state_data)
#define CMD_ID_GET_ENABLE \
    _IOWR(RTG_SCHED_IPC_MAGIC, GET_ENABLE, struct rtg_enable_data)

__attribute__((constructor))void BasicOpenRtgNode()
{
    char fileName[] = "/proc/self/sched_rtg_ctrl";
    g_fd = open(fileName, O_RDWR);
    if (g_fd < 0) {
        RME_LOGI("rtg Open fail, errno = %{public}d(%{public}s), dev = %{public}s", errno, strerror(errno), fileName);
        return;
    }
    RME_LOGI("rtg Open success");
    return;
}

__attribute__((destructor))void BasicCloseRtgNode()
{
    if (g_fd < 0) {
        return;
    }
    RME_LOGI("rtg Close g_fd ret is %{public}d", g_fd);
    close(g_fd);
}

int EnableRtg(bool flag)
{
    struct rtg_enable_data enableData;
    char configStr[] = "load_freq_switch:1;sched_cycle:1";
    enableData.enable = flag;
    enableData.len = sizeof(configStr);
    enableData.data = configStr;
    if (g_fd < 0) {
        return g_fd;
    }
    if (ioctl(g_fd, CMD_ID_SET_ENABLE, &enableData)) {
        RME_LOGE("set rtg config enable failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("set rtg config enable success.");
    }
    return 0;
};

int CreateNewRtgGrp(int prioType, int rtNum)
{
    struct rtg_grp_data grp_data;
    int ret;
    if (g_fd < 0) {
        RME_LOGE("Open file /proc/self/sched_rtg_ctrl, errno = %{public}d", errno);
        return g_fd;
    }
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    if ((prioType > 0) && (prioType < RTG_TYPE_MAX)) {
        grp_data.prio_type = prioType;
    }
    if (rtNum > 0) {
        grp_data.rt_cnt = rtNum;
    }
    grp_data.rtg_cmd = CMD_CREATE_RTG_GRP;
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    if (ret < 0) {
        RME_LOGE("create rtg grp failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("create rtg grp success, get rtg id %{public}d.", ret);
    }
    return ret;
}

int AddThreadToRtg(int tid, int grpId, int prioType)
{
    if (g_fd < 0) {   
        return g_fd;
    }
    struct rtg_grp_data grp_data;
    int ret;
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    grp_data.tid_num = 1;
    grp_data.tids[0] = tid;
    grp_data.grp_id = grpId;
    grp_data.rtg_cmd = CMD_ADD_RTG_THREAD;
    grp_data.prio_type = prioType;
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    return ret;
}

int AddThreadsToRtg(vector<int> tids, int grpId, int prioType)
{
    struct rtg_grp_data grp_data;
    int ret;
    if (g_fd < 0) {
        return g_fd;
    }
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    int num = static_cast<int>(tids.size());
    if (num > MAX_TID_NUM) {
        return -1;
    }
    grp_data.tid_num = num;
    grp_data.grp_id = grpId;
    grp_data.rtg_cmd = CMD_ADD_RTG_THREAD;
    grp_data.prio_type = prioType;
    for (int i = 0; i < num; i++) {
        if (tids[i] < 0) {
            return -1;
        }
        grp_data.tids[i] = tids[i];
    }
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    if (!ret) {
        RME_LOGI("add rtg grp success");
    }
    return ret;
};

int RemoveRtgThread(int tid)
{
    if (g_fd < 0) {
        return g_fd;
    }
    struct rtg_grp_data grp_data;
    int ret;
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    grp_data.tid_num = 1;
    grp_data.tids[0] = tid;
    grp_data.rtg_cmd = CMD_REMOVE_RTG_THREAD;
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    if (ret < 0) {
        RME_LOGE("remove grp failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("remove grp success, get rtg id %{public}d.", ret);
    }
    return ret;
};

int ClearRtgGrp(int grpId)
{
    if (g_fd < 0) {
        return g_fd;
    }
    struct rtg_grp_data grp_data;
    int ret;
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    grp_data.rtg_cmd = CMD_CLEAR_RTG_GRP;
    grp_data.grp_id = grpId;
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    if (ret < 0) {
        RME_LOGE("clear rtg grp failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("clear rtg grp success, get rtg id %{public}d.", ret);
    }
    return ret;
};

int DestroyRtgGrp(int grpId)
{
    if (g_fd < 0) {
        return g_fd;
    }
    struct rtg_grp_data grp_data;
    int ret;
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    grp_data.rtg_cmd = CMD_DESTROY_RTG_GRP;
    grp_data.grp_id = grpId;
    ret = ioctl(g_fd, CMD_ID_SET_RTG, &grp_data);
    if (ret < 0) {
        RME_LOGE("destroy rtg grp failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("destroy rtg grp success, get rtg id:%{public}d, ret:%{public}d.", grpId, ret);
    }
    return ret;
};


int SetMaxVipRtgs(int rtframe)
{
    if (g_fd < 0) {
        return g_fd;
    }
    int ret = 0;
    char str_data[MAX_STR_LEN] = {};
    (void)sprintf_s(str_data, sizeof(str_data), "rtframe:%d", rtframe);
    struct rtg_str_data strData;
    strData.len = strlen(str_data);
    strData.data = str_data;

    ret = ioctl(g_fd, CMD_ID_SET_CONFIG, &strData);
    if (ret < 0) {
        RME_LOGE("set single config failed, ret = %{public}d", ret);
    } else {
        RME_LOGI("set single config success, get rtg id %{public}d.", ret);
    }
    return ret;
}

int SetFrameRateAndPrioType(int rtgId, int rate, int rtgType)
{
    if (g_fd < 0) {
        return g_fd;
    }
    int ret = 0;
    char str_data[MAX_LENGTH] = {};
    (void)sprintf_s(str_data, sizeof(str_data), "rtgId:%d;rate:%d;type:%d", rtgId, rate, rtgType);
    struct rtg_str_data strData;
    strData.len = strlen(str_data);
    strData.data = str_data;

    ret = ioctl(g_fd, CMD_ID_SET_RTG_ATTR, &strData);
    if (ret < 0) {
        RME_LOGE("set rtg attr failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("set rtg attr success, get rtg id %{public}d.", ret);
    }
    return ret;
}

int BeginFrameFreq(int grpId, int stateParam)
{
    if (g_fd < 0) {
        return g_fd;
    }
    int ret = 0;
    struct proc_state_data state_data;
    state_data.grp_id = grpId;
    state_data.state_param = stateParam;

    ret = ioctl(g_fd, CMD_ID_BEGIN_FRAME_FREQ, &state_data);
    return ret;
}

int EndFrameFreq(int grpId)
{
    if (g_fd < 0) {
        return g_fd;
    }
    int ret = 0;
    struct proc_state_data state_data;
    state_data.grp_id = grpId;
    state_data.state_param = 0;

    ret = ioctl(g_fd, CMD_ID_END_FRAME_FREQ, &state_data);
    return ret;
}

int EndScene(int grpId)
{
    int ret = 0;
    if (g_fd < 0) {
        RME_LOGE("Open fail /proc/self/sched_rtg_ctrl");
        return g_fd;
    }
    struct proc_state_data state_data;
    state_data.grp_id = grpId;

    ret = ioctl(g_fd, CMD_ID_END_SCENE, &state_data);
    if (ret >= 0) {
        RME_LOGI("set EndScene success, get ret %{public}d.", ret);
    }
    return ret;
}

int SetMinUtil(int grpId, int stateParam)
{
    int ret = 0;
    struct proc_state_data state_data;
    state_data.grp_id = grpId;
    state_data.state_param = stateParam;

    if (g_fd < 0) {
        return g_fd;
    }
    ret = ioctl(g_fd, CMD_ID_SET_MIN_UTIL, &state_data);
    if (ret < 0) {
        RME_LOGE("set min util failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("set min util success, get ret %{public}d.", ret);
    }
    return ret;
}

int SetMaxUtil(int grpId, int stateParam)
{
    return 0;
}

int SetMargin(int grpId, int stateParam)
{
    int ret = 0;
    struct proc_state_data state_data;
    state_data.grp_id = grpId;
    state_data.state_param = stateParam;

    if (g_fd < 0) {
        return g_fd;
    }
    ret = ioctl(g_fd, CMD_ID_SET_MARGIN, &state_data);
    return ret;
}

int ListRtgGroup(vector<int> *rs)
{
    int ret = 0;
    struct rtg_info rtg_info;
    if (g_fd < 0) {
        return g_fd;
    }
    if (!rs) {
        return -1;
    }
    (void)memset_s(&rtg_info, sizeof(struct rtg_info), 0, sizeof(struct rtg_info));
    ret = ioctl(g_fd, CMD_ID_LIST_RTG, &rtg_info);
    if (ret < 0) {
        RME_LOGE("list rtg group failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("list rtg group success with num %{public}d", rtg_info.rtg_num);
        rs->clear();
        for (int i = 0; i < rtg_info.rtg_num; i++) {
            rs->push_back(rtg_info.rtgs[i]);
        }
    }
    return ret;
}

int ListRtgThread(int grpId, vector<int> *rs)
{
    if (g_fd < 0) {
        return g_fd;
    }
    int ret = 0;
    struct rtg_grp_data grp_data;
    if (!rs) {
        return -1;
    }
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    grp_data.grp_id = grpId;
    ret = ioctl(g_fd, CMD_ID_LIST_RTG_THREAD, &grp_data);
    if (ret < 0) {
        RME_LOGE("list rtg thread failed, errno = %{public}d (%{public}s)", errno, strerror(errno));
    } else {
        RME_LOGI("list rtg thread success with tid num %{public}d", grp_data.tid_num);
        rs->clear();
        for (int i = 0; i < grp_data.tid_num; i++) {
            rs->push_back(grp_data.tids[i]);
        }
    }
    return ret;
}

int SearchRtgForTid(int tid)
{
    if (g_fd < 0) {
        return g_fd;
    }
    if (tid <= 0) {
        RME_LOGI("Search tid err: invalid tid.");
        return -1;
    }
    int ret = 0;
    struct proc_state_data search_data;
    (void)memset_s(&search_data, sizeof(struct proc_state_data), 0, sizeof(struct proc_state_data));
    search_data.state_param = tid;
    ret = ioctl(g_fd, CMD_ID_SEARCH_RTG, &search_data);
    if (ret >= 0) {
        RME_LOGD("Search tid %{public}d success with rtg_grp %{public}d", tid, ret);
    }
    return ret;
}

int GetRtgEnable()
{
    if (g_fd < 0) {
        return g_fd;
    }
    struct rtg_enable_data enableData;
    return ioctl(g_fd, CMD_ID_GET_ENABLE, &enableData);
}
} // namespace RME
} // namespace OHOS
