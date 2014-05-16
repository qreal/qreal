#pragma once

#include <QtCore/QTimer>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <interpreterBase/robotModel/commonRobotModel.h>
#include <support/dummyDevice.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterBaseTests {

/// Tests for CommonRobotModel.
class CommonRobotModelTest : public testing::Test
{
public:

	/// Partially mocked robot model.
	class CommonRobotModelDescendantMock : public interpreterBase::robotModel::CommonRobotModel
	{
	public:
		/// Constructor.
		/// @param immediateConnection - if true, model connects to robot without delay, as 2d model, if false --- after
		///        some timeout.
		explicit CommonRobotModelDescendantMock(bool immediateConnection = false);

		MOCK_CONST_METHOD0(name, QString());
		MOCK_CONST_METHOD0(friendlyName, QString());
		MOCK_CONST_METHOD0(needsConnection, bool());

		void connectToRobot() override;

		MOCK_CONST_METHOD0(convertibleBases, QList<interpreterBase::robotModel::DeviceInfo>());

	private:
		interpreterBase::robotModel::robotParts::Device * createDevice(
				interpreterBase::robotModel::PortInfo const &port
				, interpreterBase::robotModel::DeviceInfo const &deviceInfo
				) override;

		QTimer mConnectionTimer;
		bool mImmediateConnection;
	};
};

}
}
}
