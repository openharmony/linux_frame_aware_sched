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
#include "qos.h"
#include <cstdio>
#include <unistd.h>

using namespace testing;
using namespace testing::ext;
using namespace OHOS::QOS;

class QosManagerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void QosManagerTest::SetUpTestCase()
{
}

void QosManagerTest::TearDownTestCase()
{
}

void QosManagerTest::SetUp()
{
}

void QosManagerTest::TearDown()
{
}

/**
 * @tc.name: QosManagerTest
 * @tc.desc: Verify the CreateAndDestroy function.
 * @tc.type: FUNC
 */
HWTEST_F(QosManagerTest, SetThreadQosTest1, TestSize.Level1)
{
    int ret = SetThreadQos(QosLevel::qos_user_initiated);
    EXPECT_EQ(ret, 0);
    ret = SetThreadQos(QosLevel::qos_default);
    EXPECT_EQ(ret, 0);
    ret = SetThreadQos(QosLevel::qos_utility);
    EXPECT_EQ(ret, 0);
    ret = SetThreadQos(QosLevel::qos_background);
    EXPECT_EQ(ret, 0);
}

HWTEST_F(QosManagerTest, SetThreadQosTest3, TestSize.Level1)
{
    int ret = SetQosForOtherThread(QosLevel::qos_user_initiated, gettid());
    EXPECT_EQ(ret, 0);
    ret = SetQosForOtherThread(QosLevel::qos_default, gettid());
    EXPECT_EQ(ret, 0);
    ret = SetQosForOtherThread(QosLevel::qos_utility, gettid());
    EXPECT_EQ(ret, 0);
    ret = SetQosForOtherThread(QosLevel::qos_background, gettid());
    EXPECT_EQ(ret, 0);
}

HWTEST_F(QosManagerTest, ResetThreadQosTest1, TestSize.Level1)
{
    int ret = SetThreadQos(QosLevel::qos_user_initiated);
    EXPECT_EQ(ret, 0);
    ret = ResetThreadQos();
    EXPECT_EQ(ret, 0);
}

HWTEST_F(QosManagerTest, ResetThreadQosTest2, TestSize.Level1)
{
    int ret = SetQosForOtherThread(QosLevel::qos_user_initiated, gettid());
    EXPECT_EQ(ret, 0);
    ret = ResetQosForOtherThread(gettid());
    EXPECT_EQ(ret, 0);
}