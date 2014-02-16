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

	QHash<PortInfo, PluggableDeviceInfo> devices;
	devices.insert(PortInfo("1")
			, PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());

	model.connectToRobot();

	protocolTester.wait(120);

	/// @todo We can not configure devices when model is not connected, or it will not configure them on reconnect.
	model.configureDevices(devices);

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("disconnected"));
}

/// Test for 2d model lifecycle --- it does not need connection to robot, but still has to emit correct signals.
TEST_F(CommonRobotModelTest, twoDLifecycleTest)
{
	CommonRobotModelDescendantMock model(true);

	SignalsTester protocolTester(SignalsTester::inOrder);

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::allDevicesConfigured, "allDevicesConfigured");

	QHash<PortInfo, PluggableDeviceInfo> devices;
	devices.insert(PortInfo("1")
			, PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());

	model.connectToRobot();

	model.configureDevices(devices);

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::disconnected, "disconnected");
	model.disconnectFromRobot();

	ASSERT_TRUE(protocolTester.isSignalEmitted("disconnected"));
}

/// Check "real" model behavior when there are no devices to configure.
TEST_F(CommonRobotModelTest, realNoSensorsLifecycleTest)
{
	CommonRobotModelDescendantMock model;

	SignalsTester protocolTester(SignalsTester::inOrder);

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::allDevicesConfigured, "allDevicesConfigured");

	model.connectToRobot();

	protocolTester.wait(120);

	/// @todo Ugly
	model.configureDevices(QHash<PortInfo, PluggableDeviceInfo>());

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
}

/// Check "2d" model behavior when there are no devices to configure.
TEST_F(CommonRobotModelTest, twoDNoSensorsLifecycleTest)
{
	CommonRobotModelDescendantMock model(true);

	SignalsTester protocolTester(SignalsTester::inOrder);

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::allDevicesConfigured, "allDevicesConfigured");

	model.connectToRobot();

	/// @todo Ugly
	model.configureDevices(QHash<PortInfo, PluggableDeviceInfo>());

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
}
