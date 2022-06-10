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
#include <map>
#include <iostream>
#include "frame_msg_intf.h"
#include "app_info.h"
#undef private
#undef protected

namespace OHOS {
namespace RME {
using namespace testing;
using namespace testing::ext;

class FrameMsgIntfTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void FrameMsgIntfTest::SetUpTestCase()
{
}

void FrameMsgIntfTest::TearDownTestCase()
{
}

void FrameMsgIntfTest::SetUp()
{
    bool ret = FrameMsgIntf::GetInstance().Init();
    EXPECT_EQ(ret, true);
}

void FrameMsgIntfTest::TearDown()
{
    FrameMsgIntf::GetInstance().Stop();
    std::shared_ptr<AppExecFwk::EventHandler> thread = FrameMsgIntf::GetInstance().threadHandler_;
    EXPECT_TRUE(thread == nullptr);
}
} // namespace RME
} // namespace OHOS
