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

#include "configurationTest.h"

#include <kitBase/robotModel/configuration.h>

#include "support/dummyDevice.h"

using namespace qrTest::robotsTests::kitBaseTests;
using namespace kitBase::robotModel;

TEST_F(ConfigurationTest, configureTest)
{
	Configuration configuration;
	ConfigurationInterface &configurationInterface = configuration;

	PortInfo const portA("A", output);
	PortInfo const port1("1", input);

	configurationInterface.configureDevice(new DummyDevice(portA));
	configurationInterface.configureDevice(new DummyDevice(port1));

	PortInfo unknownPort("unknown", input);

	// Devices are pending for configuration, so they are not configured yet.
	ASSERT_EQ(nullptr, configurationInterface.device(portA));
	ASSERT_EQ(nullptr, configurationInterface.device(port1));
	ASSERT_EQ(nullptr, configurationInterface.device(unknownPort));

	configurationInterface.applyConfiguration();

	// If all devices support synchronous configuration (which is the case in this test),
	// they all shall be configured by now.
	ASSERT_NE(nullptr, configurationInterface.device(portA));
	ASSERT_NE(nullptr, configurationInterface.device(port1));
	ASSERT_EQ(nullptr, configurationInterface.device(unknownPort));
}
