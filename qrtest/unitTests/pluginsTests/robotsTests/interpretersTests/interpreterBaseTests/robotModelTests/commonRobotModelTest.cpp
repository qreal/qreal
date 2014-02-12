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

//	ProtocolTester protocolTester;

//	protocolTester.expectSignal(&model, &CommonRobotModelDescendantMock::connected, "connected");

	QEventLoop eventLoop;

	QTimer connectionTimer;
	connectionTimer.setInterval(1000);
	connectionTimer.setSingleShot(true);
	QObject::connect(&connectionTimer, &QTimer::timeout, [&] () {
		model.connectionDone();
	});

	QTimer checkTimer;
	checkTimer.setInterval(1200);
	checkTimer.setSingleShot(true);
	QObject::connect(&checkTimer, &QTimer::timeout, [&] () {
		eventLoop.exit(0);
	});

	checkTimer.start();

	ON_CALL(model, doConnectToRobot()).WillByDefault(Invoke([&] () {
		connectionTimer.start();
	}));

	EXPECT_CALL(model, doConnectToRobot()).Times(AtLeast(1));


	QObject::connect(&model, &CommonRobotModel::connected, [&] (bool x) {
		this->onConnected(x);
	});

	QObject::connect(&model, &CommonRobotModel::allDevicesConfigured, [&] () { this->onAllDevicesConfigured(); });
	QObject::connect(&model, &CommonRobotModel::disconnected, [&] () { this->onDisconnected(); });

	QObject::connect(&model, &CommonRobotModel::allDevicesConfigured, [&] () { model.disconnectFromRobot(); });

	{
		InSequence dummy;
		Q_UNUSED(dummy);

		EXPECT_CALL(*this, onConnected(_)).Times(1);
		EXPECT_CALL(*this, onAllDevicesConfigured()).Times(1);
		EXPECT_CALL(*this, onDisconnected()).Times(1);
	}

	model.init();

	model.configureDevice(PortInfo("1")
			, PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::TouchSensor>());

	model.connectToRobot();

	eventLoop.exec();

//	ASSERT_TRUE(protocolTester.allIsGood());
}
