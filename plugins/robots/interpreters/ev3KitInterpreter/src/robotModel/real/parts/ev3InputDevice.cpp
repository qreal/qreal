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

#include "ev3InputDevice.h"

#include <utils/tracer.h>

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;
using namespace utils;

Ev3InputDevice::Ev3InputDevice(
		utils::robotCommunication::RobotCommunicator &robotCommunicator
		, const kitBase::robotModel::PortInfo &port
		, SensorTypeEnum lowLevelSensorType
		, INPUT_DEVICE_SUBCODE sensorMode)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
{
}

void Ev3InputDevice::send(QByteArray command, const unsigned responseSize, QByteArray reading)
{
	mRobotCommunicator.send(command, responseSize, reading);
}

char Ev3InputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}
