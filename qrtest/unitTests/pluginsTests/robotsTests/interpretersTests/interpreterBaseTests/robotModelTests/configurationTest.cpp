#include "configurationTest.h"

#include "interpreterBase/robotModel/configuration.h"

#include "support/dummyPluggableDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;

TEST_F(ConfigurationTest, configureTest)
{
	Configuration configuration;
	ConfigurationInterface &configurationInterface = configuration;

	PortInfo portA("A", {}, PortInfo::output);
	PortInfo port1("1", {}, PortInfo::input);

	configurationInterface.configureDevice(new DummyPluggableDevice(portA));
	configurationInterface.configureDevice(new DummyPluggableDevice(port1));

	PortInfo unknownPort("unknown", {}, PortInfo::inOut);

	// Devices are pending for configuration, so they are not configured yet.
	ASSERT_EQ(nullptr, configurationInterface.pluggableDevice(portA));
	ASSERT_EQ(nullptr, configurationInterface.pluggableDevice(port1));
	ASSERT_EQ(nullptr, configurationInterface.pluggableDevice(unknownPort));

	configurationInterface.unlockConfiguring();

	// If all devices support synchronous configuration (which is the case in this test),
	// they all shall be configured by now.
	ASSERT_NE(nullptr, configurationInterface.pluggableDevice(portA));
	ASSERT_NE(nullptr, configurationInterface.pluggableDevice(port1));
	ASSERT_EQ(nullptr, configurationInterface.pluggableDevice(unknownPort));
}
