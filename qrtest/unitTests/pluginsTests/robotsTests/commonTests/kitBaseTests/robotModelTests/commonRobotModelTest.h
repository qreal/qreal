#pragma once

#include <QtCore/QTimer>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <kitBase/robotModel/commonRobotModel.h>
#include <support/dummyDevice.h>

namespace qrTest {
namespace robotsTests {
namespace kitBaseTests {

/// Tests for CommonRobotModel.
class CommonRobotModelTest : public testing::Test
{
public:

	/// Partially mocked robot model.
	class CommonRobotModelDescendantMock : public kitBase::robotModel::CommonRobotModel
	{
	public:
		/// Constructor.
		/// @param immediateConnection - if true, model connects to robot without delay, as 2d model, if false --- after
		///        some timeout.
		explicit CommonRobotModelDescendantMock(bool immediateConnection = false);

		MOCK_CONST_METHOD0(name, QString());
		MOCK_CONST_METHOD0(friendlyName, QString());
		MOCK_CONST_METHOD0(needsConnection, bool());
		MOCK_CONST_METHOD0(priority, int());

		void connectToRobot() override;

		MOCK_CONST_METHOD0(convertibleBases, QList<kitBase::robotModel::DeviceInfo>());

	private:
		kitBase::robotModel::robotParts::Device * createDevice(
				kitBase::robotModel::PortInfo const &port
				, kitBase::robotModel::DeviceInfo const &deviceInfo
				) override;

		QTimer mConnectionTimer;
		bool mImmediateConnection;
	};
};

}
}
}
