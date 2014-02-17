#include "configurationTest.h"

#include "interpreterBase/robotModel/configuration.h"

#include "support/dummyDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;

TEST_F(ConfigurationTest, configureTest)
{
	Configuration configuration;
	ConfigurationInterface &configurationInterface = configuration;

	PortInfo portA("A");
	PortInfo port1("1");

	configurationInterface.configureDevice(new DummyDevice(portA));
	configurationInterface.configureDevice(new DummyDevice(port1));

	PortInfo unknownPort("unknown");

	// Devices are pending for configuration, so they are not configured yet.
	ASSERT_EQ(nullptr, configurationInterface.device(portA));
	ASSERT_EQ(nullptr, configurationInterface.device(port1));
	ASSERT_EQ(nullptr, configurationInterface.device(unknownPort));

	configurationInterface.unlockConfiguring();

	// If all devices support synchronous configuration (which is the case in this test),
	// they all shall be configured by now.
	ASSERT_NE(nullptr, configurationInterface.device(portA));
	ASSERT_NE(nullptr, configurationInterface.device(port1));
	ASSERT_EQ(nullptr, configurationInterface.device(unknownPort));
}
