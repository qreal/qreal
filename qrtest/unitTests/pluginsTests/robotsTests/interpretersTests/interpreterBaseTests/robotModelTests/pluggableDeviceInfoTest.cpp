#include "pluggableDeviceInfoTest.h"

#include <interpreterBase/robotModel/pluggableDeviceInfo.h>
#include <interpreterBase/robotModel/robotParts/abstractSensor.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

TEST_F(PluggableDeviceInfoTest, equalityTest)
{
	PluggableDeviceInfo const pluggableDevice = PluggableDeviceInfo::create<PluggableDevice>();
	PluggableDeviceInfo const sensor1 = PluggableDeviceInfo::create<AbstractSensor>();
	PluggableDeviceInfo const sensor2 = PluggableDeviceInfo::create<AbstractSensor>();
	PluggableDeviceInfo const motor = PluggableDeviceInfo::create<Motor>();

	ASSERT_EQ(sensor1, sensor2);
	ASSERT_TRUE(sensor1.isA(sensor1));
	ASSERT_TRUE(sensor1.isA(pluggableDevice));
	ASSERT_FALSE(pluggableDevice.isA(sensor1));
	ASSERT_FALSE(motor.isA(sensor1));
}
