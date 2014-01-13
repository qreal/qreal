#include "pluggableDeviceInfoTest.h"

#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;

TEST_F(PluggableDeviceInfoTest, equalityTest)
{
	PluggableDeviceInfo const nxtTouchSensor1("nxtKit", "touch", "Friendly Name Is Ignored");
	PluggableDeviceInfo const nxtTouchSensor2("nxtKit", "touch", "Touch Sensor With Other Friendly Name");
	PluggableDeviceInfo const nxtGyroscope("nxtKit", "gyro", "Gyroscope");
	PluggableDeviceInfo const trikTouchSensor1("trik", "touch", "Trik Touch Sensor");
	PluggableDeviceInfo const emptyDevice("", "", "");

	ASSERT_EQ(nxtTouchSensor1, nxtTouchSensor2);
	ASSERT_NE(nxtTouchSensor1, nxtGyroscope);
	ASSERT_NE(nxtTouchSensor1, trikTouchSensor1);
	ASSERT_NE(trikTouchSensor1, emptyDevice);
}
