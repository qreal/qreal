#include "pluggableDeviceTest.h"

#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;

using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

TEST_F(PluggableDeviceTest, portTest)
{
	PortInfo port("JM1", {"A"}, PortInfo::output);
	PluggableDevice device(port);

	PortInfo result = device.port();
	ASSERT_EQ(port.direction(), result.direction());
	ASSERT_EQ(port.name(), result.name());
	ASSERT_EQ(port.nameAliases(), result.nameAliases());
}
