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

#include "frame_msg_intf.h"
#include "ffrt_inner.h"
#include "intellisense_server.h"
#include "rme_log_domain.h"

#undef LOG_TAG
#define LOG_TAG "ueaServer-FrameMsgIntf"

namespace OHOS {
namespace RME {

FrameMsgIntf& FrameMsgIntf::GetInstance()
{
    static FrameMsgIntf instance;
    return instance;
}

FrameMsgIntf::~FrameMsgIntf()
{
    if (taskQueue_ != nullptr) {
        delete taskQueue_;
        taskQueue_ = nullptr;
    }
}

bool FrameMsgIntf::Init()
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    RME_LOGI("init begin!");
    if (!GetThreadQueue()) {
        return false;
    }
    taskQueue_->submit([] {
        IntelliSenseServer::GetInstance().Init();
    });
    return true;
}

bool FrameMsgIntf::GetThreadQueue()
{
    if (taskQueue_  == nullptr) {
        taskQueue_  = new(ffrt::queue)("frame_aware_sched_msg_queue",
            ffrt::queue_attr().qos(ffrt::qos_user_interactive));
        if (taskQueue_  == nullptr) {
            RME_LOGE("failed to create taskQueue!");
            return false;
        }
    }
    RME_LOGI("Init process success!");
    return true;
}

void FrameMsgIntf::ReportWindowFocus(const int pid, const int uid, const int isFocus)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    if (taskQueue_ == nullptr) {
        RME_LOGE("[ReportWindowFocus]:taskQueue none!");
        return;
    }
    taskQueue_->submit([pid, uid, isFocus] {
        IntelliSenseServer::GetInstance().ReportWindowFocus(pid, uid, isFocus);
    });
}

void FrameMsgIntf::ReportRenderThread(const int pid, const int uid, const int renderTid)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    RME_LOGI("[ReportRenderThread]:render get %{public}d with render %{pubilc}d", pid, renderTid);
    if (taskQueue_ == nullptr) {
        RME_LOGE("[ReportRenderThread]:taskQueue none!");
        return;
    }
    taskQueue_->submit([pid, uid, renderTid] {
        IntelliSenseServer::GetInstance().ReportRenderThread(pid, uid, renderTid);
    });
}

void FrameMsgIntf::ReportAppInfo(const int pid, const int uid, const std::string bundleName, ThreadState state)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    if (taskQueue_ == nullptr) {
        RME_LOGI("[ReportAppInfo]:taskQueue none!");
        return;
    }
    RME_LOGI("ReportProcessInfo pid is %{public}d, uid is %{public}d", pid, uid);
    taskQueue_->submit([pid, uid, bundleName, state] {
        IntelliSenseServer::GetInstance().ReportAppInfo(pid, uid, bundleName, state);
    });
}

void FrameMsgIntf::ReportProcessInfo(const int pid, const int uid, const std::string bundleName, ThreadState state)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    if (taskQueue_ == nullptr) {
        RME_LOGI("[ReportProcessInfo]:taskQueue none!");
        return;
    }
    taskQueue_->submit([pid, uid, bundleName, state] {
        IntelliSenseServer::GetInstance().ReportProcessInfo(pid, uid, bundleName, state);
    });
}

void FrameMsgIntf::ReportCgroupChange(const int pid, const int uid, const int oldGroup, const int newGroup)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    if (taskQueue_ == nullptr) {
        RME_LOGI("[ReportProcessInfo]:taskQueue none!");
        return;
    }
    RME_LOGI("CgroupChanged pid is %{public}d, uid is %{public}d, oldGroup is %{public}d, newGroup is %{public}d",
        pid, uid, oldGroup, newGroup);
    taskQueue_->submit([pid, uid, oldGroup, newGroup] {
        IntelliSenseServer::GetInstance().ReportCgroupChange(pid, uid, oldGroup, newGroup);
    });
}

void FrameMsgIntf::ReportContinuousTask(const int pid, const int uid, const int status)
{
    std::lock_guard<ffrt::mutex> autoLock(frameMsgIntfMutex_);
    if (taskQueue_ == nullptr) {
        RME_LOGI("[ReportProcessInfo]:taskQueue none!");
        return;
    }
    taskQueue_->submit([pid, uid, status] {
        IntelliSenseServer::GetInstance().ReportContinuousTask(pid, uid, status);
    });
}

void FrameMsgIntf::ReportSlideEvent(const int pid, const int uid, const int64_t status)
{
    return;
}

void FrameMsgIntf::Stop()
{
    return;
}
} // namespace RME
} // namespace OHOS

