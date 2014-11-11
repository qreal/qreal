#include "deviceTest.h"

#include "support/dummyDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;

using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

TEST_F(DeviceTest, portTest)
{
	PortInfo port("JM1", output, {"A"});
	DummyDevice device(port);

	PortInfo result = device.port();
	ASSERT_EQ(port.name(), result.name());
	ASSERT_EQ(port.nameAliases(), result.nameAliases());
}
