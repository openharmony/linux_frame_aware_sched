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

#include "rtg_msg_mgr.h"

namespace OHOS {
namespace RME {
namespace {
    constexpr int PRIO_TYPE = 0;
    constexpr int RT_NUM = 0;
}
DEFINE_RMELOG_INTELLISENSE("ueaServer-RtgMsgMgr");
IMPLEMENT_SINGLE_INSTANCE(RtgMsgMgr);

void RtgMsgMgr::Init()
{
    int ret = EnableRtg(true);
    if (ret < 0) {
        RME_LOGE("[Init]: enable rtg failed!");
    }
}

int RtgMsgMgr::OnForeground(const std::string appName, const int pid)
{   
    // for multiwindow
    int rtGrp = CreateNewRtgGrp(PRIO_TYPE, RT_NUM);
    if (rtGrp <= 0) {
        RME_LOGE("[OnForeground]: createNewRtgGroup failed! rtGrp:%{public}d, pid: %{public}d", rtGrp, pid);
        return rtGrp;
    }
    int ret = AddThreadToRtg(pid, rtGrp); // add ui thread
    if (ret != 0) {
        RME_LOGI("[OnForeground]: add thread pid: %{public}d to rtgGrp: %{public}d failed! ret: %{publid}d", pid, rtGrp, ret);
    }
    return rtGrp;
}

void RtgMsgMgr::OnBackground(const std::string appName, const int pid, const int grpId)
{
    if (grpId <= 0) {
        RME_LOGE("[OnBackground]:do not find grpid, pid:%{public}d, grpId:%{public}d", pid, grpId);
        return;
    }
    DestroyRtgGrp(grpId);
}

void RtgMsgMgr::ProcessStart(const int tid, const int grpId)
{
    if (grpId <= 0) {
        RME_LOGE("[ProcessStart]:do not find grpid, tid:%{public}d, grpId:%{public}d", tid, grpId);
        return;
    }
    AddThreadToRtg(tid, grpId);
}

void RtgMsgMgr::ProcessDied(const int pid, const int tid)
{
    int removeTid = -1;
    if (tid == -1) {
        removeTid = pid;
    } else {
        removeTid = tid;
    }
    int ret = RemoveRtgThread(removeTid);
    if (ret < 0) {
        RME_LOGE("[ProcessDied]: removeRtgGroup failed!pid:%{public}d, tid:%{public}d", pid, tid);
    } else {
        RME_LOGI("[ProcessDied]: removeRtgGroup success! pid:%{public}d, tid:%{public}d:tid", pid, tid);
    }

}

void RtgMsgMgr::FpsChanged()
{
    RME_LOGI("[FpsChanged]: need to change fps for rtggrp!");
}

void RtgMsgMgr::FocusChanged(const int pid, bool isFocus)
{
    RME_LOGI("[FocusChanged]: need to change prio for rtggrp");
}
} // RME
} // OHOS
