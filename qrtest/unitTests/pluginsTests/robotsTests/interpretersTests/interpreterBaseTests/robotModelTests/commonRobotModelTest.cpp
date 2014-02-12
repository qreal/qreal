#include "commonRobotModelTest.h"

#include <QtCore/QEventLoop>
#include <QtCore/QDebug>

#include <interpreterBase/robotModel/robotParts/touchSensor.h>

#include "utils/protocolTester.h"

using namespace qrTest;
using namespace qrTest::robotsTests::interpreterBaseTests;

using namespace interpreterBase;
using namespace interpreterBase::robotModel;

using namespace ::testing;

TEST_F(CommonRobotModelTest, initializationTest)
{
	CommonRobotModelDescendantMock model;
}

TEST_F(CommonRobotModelTest, lifecycleTest)
{
	CommonRobotModelDescendantMock model;

	ProtocolTester protocolTester;

	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::disconnected, "disconnected");
	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::allDevicesConfigured, "allDevicesConfigured");

	QTimer connectionTimer;
	connectionTimer.setInterval(1000);
	connectionTimer.setSingleShot(true);
	QObject::connect(&connectionTimer, &QTimer::timeout, [&] () {
		model.connectionDone();
	});


	ON_CALL(model, doConnectToRobot()).WillByDefault(Invoke([&] () {
		connectionTimer.start();
	}));

	EXPECT_CALL(model, doConnectToRobot()).Times(AtLeast(1));

	QObject::connect(&model, &CommonRobotModel::allDevicesConfigured, [&] () { model.disconnectFromRobot(); });

	model.init();

	model.configureDevice(PortInfo("1")
			, PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());

	model.connectToRobot();

	protocolTester.wait(1200);

	ASSERT_TRUE(protocolTester.isSignalEmitted("connected"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("allDevicesConfigured"));
	ASSERT_TRUE(protocolTester.isSignalEmitted("disconnected"));
}
