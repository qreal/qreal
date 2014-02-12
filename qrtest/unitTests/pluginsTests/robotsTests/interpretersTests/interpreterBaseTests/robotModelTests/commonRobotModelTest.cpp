#include "commonRobotModelTest.h"

#include <interpreterBase/robotModel/robotParts/touchSensor.h>

#include "utils/signalsTester.h"

using namespace qrTest;
using namespace qrTest::robotsTests::interpreterBaseTests;

using namespace interpreterBase;
using namespace interpreterBase::robotModel;

using namespace ::testing;

/// Basic model constructor check.
TEST_F(CommonRobotModelTest, initializationTest)
{
	CommonRobotModelDescendantMock model;
}

/// Test for general model lifecycle --- creation, initialization, connecting, configuration and disconnecting, if all
/// is well.
TEST_F(CommonRobotModelTest, lifecycleTest)
{
	CommonRobotModelDescendantMock model;

	SignalsTester protocolTester(SignalsTester::inOrder);

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::allDevicesConfigured, "allDevicesConfigured");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::disconnected, "disconnected");

	QObject::connect(&model, &CommonRobotModel::allDevicesConfigured
			, [&] () {
				model.disconnectFromRobot();
			});

	model.init();

	model.configureDevice(PortInfo("1")
			, PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());

	model.connectToRobot();

	protocolTester.wait(1200);

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("disconnected"));
}
