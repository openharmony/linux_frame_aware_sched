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

#ifndef OHOS_FRAME_AWARE_SCHED_COMMON_INCLUDE_RTG_INTERFACE_H
#define OHOS_FRAME_AWARE_SCHED_COMMON_INCLUDE_RTG_INTERFACE_H

#include <string>
#include <vector>

namespace OHOS {
namespace RME {
using namespace std;

#define MAX_TID_NUM 5
#define MAX_SUBPROCESS_NUM 8
#define MULTI_FRAME_NUM 5

struct rtg_enable_data {
    int enable;
    int len;
    char *data;
};

struct rtg_str_data {
    int type;
    int len;
    char *data;
};

struct proc_state_data {
    int grp_id;
    int state_param;
};

enum grp_ctrl_cmd {
    CMD_CREATE_RTG_GRP,
    CMD_ADD_RTG_THREAD,
    CMD_REMOVE_RTG_THREAD,
    CMD_CLEAR_RTG_GRP,
    CMD_DESTROY_RTG_GRP
};

struct rtg_grp_data {
    int rtg_cmd;
    int grp_id;
    int prio_type;
    int rt_cnt;
    int tid_num;
    int tids[MAX_TID_NUM];
};

struct rtg_info {
    int rtg_num;
    int rtgs[MULTI_FRAME_NUM];
};

enum rtg_sched_cmdid {
    SET_ENABLE = 1,
    SET_RTG,
    SET_CONFIG,
    SET_RTG_ATTR,
    BEGIN_FRAME_FREQ = 5,
    END_FRAME_FREQ,
    END_SCENE,
    SET_MIN_UTIL,
    SET_MARGIN,
    LIST_RTG,
    LIST_RTG_THREAD,
    SEARCH_RTG,
    GET_ENABLE,
    RTG_CTRL_MAX_NR,
};

extern "C" {
    int EnableRtg(bool flag);
    int CreateNewRtgGrp(int prioType = 0, int rtNum = 0); // Return GrpId if success
    int AddThreadToRtg(int tid, int grpId, int prioType = 0);
    int AddThreadsToRtg(vector<int> tids, int grpId, int prioType = 0);
    int RemoveRtgThread(int tid);
    int ClearRtgGrp(int grpId);
    int DestroyRtgGrp(int grpId);
    int SetFrameRateAndPrioType(int rtgId, int rate, int rtgType);
    int SetMaxVipRtgs(int rtframe);
    int BeginFrameFreq(int grpId, int stateParam);
    int EndFrameFreq(int grpId);
    int EndScene(int grpId);
    int SetMinUtil(int grpId, int stateParam);
    int SetMargin(int grpId, int stateParam);
    int ListRtgThread(int grpId, vector<int> *rs); // list all tasks of grpId;
    int ListRtgGroup(vector<int> *rs);
    int SearchRtgForTid(int stateParam);
    int GetRtgEnable();
}
} // namespace RME
} // namespace OHOS
#endif // OHOS_FRAME_AWARE_SCHED_COMMON_INCLUDE_RTG_INTERFACE_H