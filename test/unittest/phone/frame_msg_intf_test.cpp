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
    bool ret = FrameMsgIntf::GetInstance().Init();
    EXPECT_EQ(ret, true);
}

void FrameMsgIntfTest::TearDown()
{
    FrameMsgIntf::GetInstance().Stop();
    std::shared_ptr<AppExecFwk::EventHandler> thread = FrameMsgIntf::GetInstance().threadHandler_;
    EXPECT_TRUE(thread == nullptr);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportAppInfoColdStart, TestSize.Level1)
{
    int pid_1 = 1001;
    std::string appName_1 = "com.ohos.frameaware.testColdStart";
   
    // app foreground
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    bool isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    EXPECT_TRUE(isAppPidExist); // should be true
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportAppInfoWarmStart, TestSize.Level1)
{
    int pid_1 = 1002;
    std::string appName_1 = "com.ohos.frameaware.testWarmStart";
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_BACKGROUND);

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    sleep(1);

    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    bool isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    EXPECT_TRUE(isAppPidExist); // should be true
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportAppInfoTerminated, TestSize.Level1)
{
    int pid_1 = 1003;
    std::string appName_1 = "com.ohos.frameaware.testAppDied";
    // start
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    
    // terminated
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_TERMINATED);
    sleep(1);

    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    bool isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    EXPECT_FALSE(isAppPidExist);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportAppInfoSwitch, TestSize.Level1)
{
    int pid_1 = 1004;
    std::string appName_1 = "com.ohos.frameaware.testAppSwitch1";
    int pid_2 = 10041;
    std::string appName_2 = "com.ohos.frameaware.testAppSwitch2";

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_2, appName_2, pid_2, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_BACKGROUND);

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_2, appName_2, pid_2, AppStateUpdateReason::APP_BACKGROUND);
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    sleep(1);

    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    bool isAppPidExist = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_1);
    bool isAppPidExist2 = AppInfoMgr::GetInstance().GetForegroundApp().count(pid_2);
    EXPECT_TRUE(isAppPidExist); // should be true
    EXPECT_FALSE(isAppPidExist2); // should be false
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportWindowFocus, TestSize.Level1)
{
    int pid_1 = 1005;
    std::string appName_1 = "com.ohos.frameaware.testWindowFocus";

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 0);
    sleep(1);
    std::shared_ptr<AppInfo> app_1 = AppInfoMgr::GetInstance().GetFocusApp();
    bool focusSta_1 = app_1->GetFocusState();
    EXPECT_EQ(focusSta_1, false); // 0 means focus
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportWindowUnFocus, TestSize.Level1)
{
    int pid_1 = 1006;
    std::string appName_1 = "com.ohos.frameaware.testWindowUnfocus";

    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_FOREGROUND);
    FrameMsgIntf::GetInstance().ReportAppInfo(appName_1, appName_1, pid_1, AppStateUpdateReason::APP_BACKGROUND);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 0);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 1);
    sleep(1);
    std::shared_ptr<AppInfo> app_1 = AppInfoMgr::GetInstance().GetFocusApp();
    EXPECT_EQ(app_1 == nullptr, true); // 1 means unfocus
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportProcessInfoUiDiedFore, TestSize.Level1)
{
    int pid_1 = 1007;
    int ui_tid = pid_1;

    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 0);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    EXPECT_EQ(mapList.count(pid_1), 1);

    FrameMsgIntf::GetInstance().ReportProcessInfo(pid_1, ui_tid, ThreadState::DIED);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList1 = AppInfoMgr::GetInstance().GetForegroundApp();
    EXPECT_EQ(mapList1.count(pid_1), 0);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportProcessInfoUiDiedBack, TestSize.Level1)
{
    int pid_1 = 1008;
    int ui_tid = pid_1;

    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 0);
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_1, 1);
    FrameMsgIntf::GetInstance().ReportProcessInfo(pid_1, ui_tid, ThreadState::DIED);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    EXPECT_EQ(mapList.count(pid_1), 0);
}

HWTEST_F(FrameMsgIntfTest, FrameMsgIntfReportProcessInfoRenderDiedFore, TestSize.Level1)
{
    int pid_2 = 1009;
    int render_tid = 10091;
    
    FrameMsgIntf::GetInstance().ReportWindowFocus(pid_2, 0);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList = AppInfoMgr::GetInstance().GetForegroundApp();
    EXPECT_EQ(mapList.count(pid_2), 1);

    FrameMsgIntf::GetInstance().ReportProcessInfo(pid_2, render_tid, ThreadState::DIED);
    sleep(1);
    std::map<int, std::shared_ptr<AppInfo>> mapList2 = AppInfoMgr::GetInstance().GetForegroundApp();
    EXPECT_EQ(mapList2.count(pid_2), 1);
}
} // namespace RME
} // namespace OHOS
