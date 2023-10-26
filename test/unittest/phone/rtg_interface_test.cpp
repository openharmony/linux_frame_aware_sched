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

#include "gtest/gtest.h"

#define private public
#define protected public
#include "rtg_interface.h"
#undef private
#undef protected

namespace OHOS {
namespace RME {
using namespace testing;
using namespace testing::ext;

#define RTG_INTERFACE_SO_PATH "/system/lib/librtg_interface.z.so"

enum rtg_type : int {
    VIP = 0,
    TOP_TASK_KEY,
    NORMAL_TASK,
    RTG_TYPE_MAX,
};

class RtgInterfaceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void RtgInterfaceTest::SetUpTestCase()
{
}

void RtgInterfaceTest::TearDownTestCase()
{
}

void RtgInterfaceTest::SetUp()
{
    // must enable rtg before use the interface
    bool ret = EnableRtg(true);
    EXPECT_EQ(ret, false);
}

void RtgInterfaceTest::TearDown()
{
    // disable rtg after use the interface
    bool ret = EnableRtg(false);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: RtgInterfaceCreateAndDestroy
 * @tc.desc: Verify the CreateAndDestroy function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceCreateAndDestroy, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceDestroyErrorGroup
 * @tc.desc: Verify Destroy function with error param.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceDestroyErrorGroup, TestSize.Level1)
{
    int ret;
    ret = DestroyRtgGrp(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddRtg
 * @tc.desc: Verify Rtg add function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddRtg, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(pid, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddErrorThread
 * @tc.desc: Verify Rtg add function with error pid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddErrorThread, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(-1, grpId);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddErrorGroup
 * @tc.desc: Verify Rtg add function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddErrorGroup, TestSize.Level1)
{
    int ret;
    int pid = getpid();
    ret = AddThreadToRtg(pid, -1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddRtgs
 * @tc.desc: Verify Rtg add multiple thread function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddRtgs, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceClearRtg
 * @tc.desc: Verify Rtg clear function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceClearRtg, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(pid, grpId);
    EXPECT_EQ(ret, 0);
    ret = ClearRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceClearErrorGroup
 * @tc.desc: Verify Rtg clear function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceClearErrorGroup, TestSize.Level1)
{
    int ret;
    ret = ClearRtgGrp(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceBeginFrameFreq
 * @tc.desc: Verify rtg frame start function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceBeginFrameFreq, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = BeginFrameFreq(grpId, 0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceBeginFrameFreqWithErrorGrp
 * @tc.desc: Verify rtg frame start function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceBeginFrameFreqWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = BeginFrameFreq(-1, 0);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndFrameFreq
 * @tc.desc: Verify rtg frame end function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndFrameFreq, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = EndFrameFreq(grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndFrameFreqWithErrorGrp
 * @tc.desc: Verify rtg frame end function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndFrameFreqWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = EndFrameFreq(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndScene
 * @tc.desc: Verify rtg frame scene end function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndScene, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = EndScene(grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndSceneWithErrorGrp
 * @tc.desc: Verify rtg frame scene end function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndSceneWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = EndScene(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMinUtil
 * @tc.desc: Verify rtg minUtil set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMinUtil, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetMinUtil(grpId, 0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMinUtilWithErrorGrp
 * @tc.desc: Verify rtg minUtil set function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMinUtilWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = SetMinUtil(-1, 0);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMargin
 * @tc.desc: Verify rtg margin set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMargin, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetMargin(grpId, 0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMarginWithErrorGrp
 * @tc.desc: Verify rtg margin set function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMarginWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = SetMargin(-1, 0);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceListRtgThread
 * @tc.desc: Verify rtg list function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceListRtgThread, TestSize.Level1)
{
    int ret;
    int grpId;
    vector<int> rs;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = ListRtgThread(grpId, &rs);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceListRtgThreadWithNullRes
 * @tc.desc: Verify rtg thread list function with null vector input.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceListRtgThreadWithNullRes, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = ListRtgThread(grpId, nullptr);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceListRtgThreadWithErrorGrp
 * @tc.desc: Verify rtg thread list function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceListRtgThreadWithErrorGrp, TestSize.Level1)
{
    int ret;
    vector<int> rs;
    ret = ListRtgThread(-1, &rs);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceListRtgGroup
 * @tc.desc: Verify rtg list function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceListRtgGroup, TestSize.Level1)
{
    int ret;
    vector<int> rs;
    ret = ListRtgGroup(&rs);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceListRtgGroupWithNullRes
 * @tc.desc: Verify rtg list function with null vector input.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceListRtgGroupWithNullRes, TestSize.Level1)
{
    int ret;
    ret = ListRtgGroup(nullptr);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetAttr
 * @tc.desc: Verify rtg attr set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetAttr, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetFrameRateAndPrioType(grpId, 60, VIP);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetErrorAttr
 * @tc.desc: Verify rtg attr set function with error attr param.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetErrorAttr, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetFrameRateAndPrioType(grpId, 90, -1);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMaxVips
 * @tc.desc: Verify rtg max vip num set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMaxVips, TestSize.Level1)
{
    int ret;
    ret = SetMaxVipRtgs(2);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetErrorMaxVips
 * @tc.desc: Verify rtg max vip num set function with 0 vip nums.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetErrorMaxVips, TestSize.Level1)
{
    int ret;
    ret = SetMaxVipRtgs(0);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetLargeMaxVips
 * @tc.desc: Verify rtg max vip num set function with too large vip nums.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetLargeMaxVips, TestSize.Level1)
{
    int ret;
    ret = SetMaxVipRtgs(50000);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddMultipleThreads
 * @tc.desc: Verify rtg multiple add function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddMultipleThreads, TestSize.Level1)
{
    int ret;
    int pid[3];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddMultipleThreadsOutOfLimit
 * @tc.desc: Verify rtg multiple add function with out of limit threads.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddMultipleThreadsOutOfLimit, TestSize.Level1)
{
    int ret;
    int pid[8];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 8; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}
} // namespace RME
} // namespace OHOS
