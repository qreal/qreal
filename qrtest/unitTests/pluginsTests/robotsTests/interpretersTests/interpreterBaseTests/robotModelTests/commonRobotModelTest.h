#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterBaseTests {

class CommonRobotModelTest : public testing::Test
{
public:

	class CommonRobotModelDescendantMock : public interpreterBase::robotModel::CommonRobotModel
	{
	public:
		MOCK_CONST_METHOD0(name, QString());
		MOCK_CONST_METHOD0(friendlyName, QString());
		MOCK_CONST_METHOD0(needsConnection, bool());
		MOCK_METHOD0(doConnectToRobot, void());
		MOCK_CONST_METHOD0(convertibleBases, QList<interpreterBase::robotModel::PluggableDeviceInfo>());
		void connectionDone() { onConnected(); }
	};
};

}
}
}
