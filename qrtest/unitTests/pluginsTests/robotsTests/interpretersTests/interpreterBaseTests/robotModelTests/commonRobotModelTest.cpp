#include "commonRobotModelTest.h"

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

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
	Q_UNUSED(model)
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

	QObject::connect(&model, &CommonRobotModel::connected, [&] () {
		model.configureDevice(PortInfo("1", input)
				, DeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());
		model.applyConfiguration();
	});
	QObject::connect(&model, &CommonRobotModel::allDevicesConfigured, [&] () { model.disconnectFromRobot(); });

	model.init();

	model.connectToRobot();

	protocolTester.wait(120);

	/// @todo We can not configure devices when model is not connected, or it will not configure them on reconnect.

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

	model.configureDevice(PortInfo("1", input)
			, DeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());
	model.applyConfiguration();

	model.connectToRobot();

	protocolTester.wait(1);

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

	QObject::connect(&model, &CommonRobotModel::connected, [&] () { model.applyConfiguration(); });

	model.connectToRobot();

	protocolTester.wait(120);

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

	model.applyConfiguration();

	model.connectToRobot();

	protocolTester.wait(1);

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
}

CommonRobotModelTest::CommonRobotModelDescendantMock::CommonRobotModelDescendantMock(bool immediateConnection)
	: CommonRobotModel("test", "testRobot")
	, mImmediateConnection(immediateConnection)
{
	addAllowedConnection(PortInfo("1", input)
			, { DeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>()
					, DeviceInfo::create<interpreterBase::robotModel::robotParts::RangeSensor>()});

	if (!immediateConnection) {
		mConnectionTimer.setInterval(100);
		mConnectionTimer.setSingleShot(true);
		QObject::connect(&mConnectionTimer, &QTimer::timeout, [&] () {
			emit connected(true);
		});
	}
}

void CommonRobotModelTest::CommonRobotModelDescendantMock::connectToRobot()
{
	if (!mImmediateConnection) {
		mConnectionTimer.start();
	} else {
		emit connected(true);
	}
}

robotParts::Device *CommonRobotModelTest::CommonRobotModelDescendantMock::createDevice(PortInfo const &port
		, DeviceInfo const &deviceInfo)
{
	Q_UNUSED(deviceInfo)

	return new DummyDevice(port);
}
