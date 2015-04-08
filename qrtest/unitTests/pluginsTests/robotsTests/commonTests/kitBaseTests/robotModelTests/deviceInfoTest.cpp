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
