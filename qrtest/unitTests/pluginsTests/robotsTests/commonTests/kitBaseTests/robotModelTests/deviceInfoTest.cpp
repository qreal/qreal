#include "deviceInfoTest.h"

#include <kitBase/robotModel/deviceInfo.h>
#include <kitBase/robotModel/robotParts/abstractSensor.h>
#include <kitBase/robotModel/robotParts/motor.h>

using namespace qrTest::robotsTests::kitBaseTests;
using namespace kitBase::robotModel;
using namespace kitBase::robotModel::robotParts;

TEST_F(DeviceInfoTest, equalityTest)
{
	DeviceInfo const device = DeviceInfo::create<Device>();
	DeviceInfo const sensor1 = DeviceInfo::create<AbstractSensor>();
	DeviceInfo const sensor2 = DeviceInfo::create<AbstractSensor>();
	DeviceInfo const motor = DeviceInfo::create<Motor>();

	ASSERT_EQ(sensor1, sensor2);
	ASSERT_TRUE(sensor1.isA(sensor1));
	ASSERT_TRUE(sensor1.isA(device));
	ASSERT_FALSE(device.isA(sensor1));
	ASSERT_FALSE(motor.isA(sensor1));
}
