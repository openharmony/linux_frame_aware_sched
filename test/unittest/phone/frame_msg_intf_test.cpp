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
#include "frame_msg_intf.h"
#include "app_info_mgr.h"
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
}

void FrameMsgIntfTest::TearDown()
{
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfInit, TestSize.Level1)
{
    bool ret = FrameMsgIntf::GetInstance().Init();
    EXPECT_EQ(ret, true);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportAppInfo, TestSize.Level1)
{
    int pid_1 = 12315;
    std::string appName_1 = "com.ohos.frameaware.test1";
    int pid_2 = 12366;
    std::string appName_2 = "com.ohos.frameaware.test2";
    
    // app foreground
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_2, appName_2, pid_2, AppStateUpdateReason::APP_FOREGROUND);

    // app background
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_2, appName_2, pid_2, AppStateUpdateReason::APP_BACKGROUND);

    
    bool isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    bool isAppPidExist2 = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_2);
    
    EXPECT_EQ(isAppPidExist, true);
    EXPECT_EQ(isAppPidExist2, false);

    // app terminated
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_TERMINATED);
    isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    EXPECT_EQ(isAppPidExist, false);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportWindowFocus, TestSize.Level1)
{

    int pid_1 = 12234;
    int pid_2 = 23134;
    int pid_3 = 8912;

    std::string appName_1 = "com.ohos.frameaware.test1";
    std::string appName_2 = "com.ohos.frameaware.test2";
    std::string appName_3 = "com.ohos.frameaware.test3";

    // app focus
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_2, appName_2, pid_2, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 1);

    // app Unfocus
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_3, appName_3, pid_3, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_3, 0);

    std::shared_ptr<AppInfo> app_1 = AppInfoMgr::GetInstance().GetFocusApp();
    bool focusSta_1 = app_1->GetFocusState();
    std::shared_ptr<AppInfo> app_2 = AppInfoMgr::GetInstance().GetFocusApp();
    bool focusSta_2 = app_2->GetFocusState();

    EXPECT_EQ(focusSta_1, true);
    EXPECT_EQ(focusSta_2, false);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportProcessInfo, TestSize.Level1)
{
    //process died 
    int pid_1 = 12435;
    int pid_2 = 12436;
    int ui_tid = 12435;
    int render_tid = 13332;

    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 1);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_2, 1);

    FrameMsgIntf::GetInstance().ReportProcessInfo(pid_1, ui_tid, ThreadState::DIED);
    FrameMsgIntf::GetInstance().ReportProcessInfo(pid_1, render_tid, ThreadState::DIED);

    std::shared_ptr<AppInfo> app_1 = AppInfoMgr::GetInstance().mForegroundAppList[pid_1];
    EXPECT_EQ(app_1->GetUiTid(), ui_tid);
    EXPECT_EQ(app_1->GetRenderTid(), render_tid);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfStop, TestSize.Level1)
{
    FrameMsgIntf::GetInstance().Stop();
    std::shared_ptr<AppExecFwk::EventHandler> thread = FrameMsgIntf::GetInstance().threadHandler_;
    EXPECT_TRUE(thread == nullptr); 
}

} // namespace RME
} // namespace OHOS
