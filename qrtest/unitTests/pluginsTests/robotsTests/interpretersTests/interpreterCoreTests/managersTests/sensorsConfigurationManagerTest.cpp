/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "sensorsConfigurationManagerTest.h"

#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/colorSensor.h>

#include "mocks/qrgui/mainWindow/mainWindowInterpretersInterfaceMock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore;
using namespace kitBase::robotModel;
using namespace testing;

void SensorsConfigurationManagerTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/basicTest.qrs"));
	mQrguiFacade->setActiveTab(qReal::Id::loadFromString(
		"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"));

	mManager.reset(new DevicesConfigurationManager(mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			));

	mConfigurer1.reset(new DummySensorsConfigurer("testConfigurer1"));
	mConfigurer2.reset(new DummySensorsConfigurer("testConfigurer2"));

	mManager->connectDevicesConfigurationProvider(mConfigurer1.data());
	mManager->connectDevicesConfigurationProvider(mConfigurer2.data());
}

TEST_F(SensorsConfigurationManagerTest, serializationTest)
{
	DeviceInfo const device1 = DeviceInfo::create<robotParts::TouchSensor>();
	DeviceInfo const device2 = DeviceInfo::create<robotParts::ColorSensor>();

	// Setting up initial configuration...
	mConfigurer1->configureBroadly("model1", PortInfo("1", input), device1);
	mConfigurer1->configureBroadly("model1", PortInfo("2", input), DeviceInfo());
	mConfigurer1->configureBroadly("model2", PortInfo("A", output), device1);
	mConfigurer1->configureBroadly("model2", PortInfo("B", output), device2);

	// Checking up that second configurator was affected...
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1", input)), device1);
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2", input)), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A", output)), device1);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B", output)), device2);

	// Now initial configuration must be saved into the repository.
	// Disabling auto-serialization into repo for a moment to set another configuration.
	qReal::Id const oldDiagram = mQrguiFacade->mainWindowInterpretersInterface().activeDiagram();
	mQrguiFacade->setActiveTab(qReal::Id());

	// Setting another configuration...
	mConfigurer1->configureBroadly("model1", PortInfo("1", input), DeviceInfo());
	mConfigurer1->configureBroadly("model1", PortInfo("2", input), device2);
	mConfigurer1->configureBroadly("model2", PortInfo("A", output), DeviceInfo());
	mConfigurer1->configureBroadly("model2", PortInfo("B", output), DeviceInfo());

	// Checking up that second configurator was affected...
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1", input)), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2", input)), device2);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A", output)), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B", output)), DeviceInfo());

	// This must restore configuration from repo
	mQrguiFacade->setActiveTab(oldDiagram);

	// It was an old practice to switch devices configuration when switching between tabs.
	// Now configuration is beeng modified only when .qrs projects are opened or closed.
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("1", input)), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model1", PortInfo("2", input)), device2);
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("A", output)), DeviceInfo());
	ASSERT_EQ(mConfigurer2->device("model2", PortInfo("B", output)), DeviceInfo());
}
