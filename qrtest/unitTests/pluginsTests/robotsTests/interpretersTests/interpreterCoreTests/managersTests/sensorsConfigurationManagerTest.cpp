#include "sensorsConfigurationManagerTest.h"

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>

#include "mocks/qrgui/mainwindow/mainWindowInterpretersInterfaceMock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore;
using namespace interpreterBase::robotModel;
using namespace testing;

void SensorsConfigurationManagerTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/basicTest.qrs"));
	mQrguiFacade->setActiveTab(qReal::Id::loadFromString(
		"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"));

	mManager.reset(new SensorsConfigurationManager(mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->systemEvents()
			));

	mConfigurer1.reset(new DummySensorsConfigurer("testConfigurer1"));
	mConfigurer2.reset(new DummySensorsConfigurer("testConfigurer2"));

	mManager->connectSensorsConfigurationProvider(mConfigurer1.data());
	mManager->connectSensorsConfigurationProvider(mConfigurer2.data());
}

TEST_F(SensorsConfigurationManagerTest, serializationTest)
{
	DeviceInfo const device1 = DeviceInfo::create<robotParts::TouchSensor>();
	DeviceInfo const device2 = DeviceInfo::create<robotParts::ColorSensor>();

	// Setting up initial configuration...
	mConfigurer1->configureBroadly("model1", PortInfo("1"), device1);
	mConfigurer1->configureBroadly("model1", PortInfo("2"), DeviceInfo());
	mConfigurer1->configureBroadly("model2", PortInfo("A"), device1);
	mConfigurer1->configureBroadly("model2", PortInfo("B"), device2);

	// Checking up that second configurator was affected...
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1")), device1);
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2")), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A")), device1);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B")), device2);

	// Now initial configuration must be saved into the repository.
	// Disabling auto-serialization into repo for a moment to set another configuration.
	qReal::Id const oldDiagram = mQrguiFacade->mainWindowInterpretersInterface().activeDiagram();
	mQrguiFacade->setActiveTab(qReal::Id());

	// Setting another configuration...
	mConfigurer1->configureBroadly("model1", PortInfo("1"), DeviceInfo());
	mConfigurer1->configureBroadly("model1", PortInfo("2"), device2);
	mConfigurer1->configureBroadly("model2", PortInfo("A"), DeviceInfo());
	mConfigurer1->configureBroadly("model2", PortInfo("B"), DeviceInfo());

	// Checking up that second configurator was affected...
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1")), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2")), device2);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A")), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B")), DeviceInfo());

	// This must restore configuration from repo
	mQrguiFacade->setActiveTab(oldDiagram);

	// Checking that old configuration was restored...
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1")), device1);
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2")), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A")), device1);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B")), device2);
}
