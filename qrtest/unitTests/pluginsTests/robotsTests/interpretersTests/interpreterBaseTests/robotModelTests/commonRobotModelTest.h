#pragma once

#include <QtCore/QTimer>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <interpreterBase/robotModel/commonRobotModel.h>

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
		CommonRobotModelDescendantMock(bool immediateConnection = false)
			: mImmediateConnection(immediateConnection)
		{
			if (!immediateConnection) {
				mConnectionTimer.setInterval(100);
				mConnectionTimer.setSingleShot(true);
				QObject::connect(&mConnectionTimer, &QTimer::timeout, [&] () {
					onConnected();
				});
			}
		}

		MOCK_CONST_METHOD0(name, QString());
		MOCK_CONST_METHOD0(friendlyName, QString());
		MOCK_CONST_METHOD0(needsConnection, bool());

		void doConnectToRobot() override
		{
			if (!mImmediateConnection) {
				mConnectionTimer.start();
			} else {
				onConnected();
			}
		}

		MOCK_CONST_METHOD0(convertibleBases, QList<interpreterBase::robotModel::PluggableDeviceInfo>());

	private:
		QTimer mConnectionTimer;
		bool mImmediateConnection;
	};
};

}
}
}
