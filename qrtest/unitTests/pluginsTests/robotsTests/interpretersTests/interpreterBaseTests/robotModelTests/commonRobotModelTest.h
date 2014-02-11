#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterBaseTests {

class CommonRobotModelDescendantMock : public interpreterBase::robotModel::CommonRobotModel
{
public:
	MOCK_CONST_METHOD0(name, QString());
	MOCK_CONST_METHOD0(friendlyName, QString());
	MOCK_CONST_METHOD0(needsConnection, bool());
//	MOCK_METHOD0(doConnectToRobot, void());
};

class CommonRobotModelTest : public testing::Test
{
public:
	MOCK_METHOD1(onConnected, void(bool success));
	MOCK_METHOD0(onDisconnected, void());
	MOCK_METHOD0(onAllDevicesConfigured, void());
};

}
}
}
