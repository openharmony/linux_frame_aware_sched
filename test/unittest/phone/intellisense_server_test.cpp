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

#include "gtest/gtest.h"

#define private public
#include "intellisense_server.h"
#undef private

namespace OHOS {
namespace RME {
using namespace testing;
using namespace testing::ext;

class IntelliSenseServerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void IntelliSenseServerTest::SetUpTestCase()
{
}

void IntelliSenseServerTest::TearDownTestCase()
{
}

void IntelliSenseServerTest::SetUp()
{
}

void IntelliSenseServerTest::TearDown()
{
}

/**
 * @tc.name: ReportContinuousTaskTest
 * @tc.desc: Verify the ReportContinuousTask function.
 * @tc.type: FUNC
 */
HWTEST_F(IntelliSenseServerTest, ReportContinuousTaskTest, TestSize.Level1)
{
    int pid = 777;
    int uid = 888;
    int status = 0;
    IntelliSenseServer::GetInstance().m_switch = false;
    IntelliSenseServer::GetInstance().ReportContinuousTask(pid, uid, status);
    IntelliSenseServer::GetInstance().m_switch = true;
    IntelliSenseServer::GetInstance().ReportContinuousTask(pid, uid, status);
    status = 1;
    IntelliSenseServer::GetInstance().ReportContinuousTask(pid, uid, status);
    status = 2;
    IntelliSenseServer::GetInstance().ReportContinuousTask(pid, uid, status);
}
} // namespace RME
} // namespace OHOS