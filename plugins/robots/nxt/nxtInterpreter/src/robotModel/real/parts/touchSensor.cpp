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

#include "touchSensor.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

TouchSensor::TouchSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::TouchSensor(info, port)
	, mImplementation(robotCommunicator, port, enums::lowLevelSensorType::SWITCH, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &TouchSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &TouchSensor::configurationCompleted);
}

void TouchSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	mImplementation.setState(NxtInputDevice::idle);
	int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	if (reading[4] == 0 && sensorValue < 500) {
		// Sensor is pressed.
		emit newData(1);
	} else {
		emit newData(0);
	}
}

void TouchSensor::read()
{
	if (!mImplementation.isConfigured()) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mImplementation.state() == NxtInputDevice::pending) {
		return;
	}

	mImplementation.setState(NxtInputDevice::pending);
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mImplementation.lowLevelPort();
	mImplementation.send(command, 18);
}

void TouchSensor::doConfiguration()
{
	mImplementation.configure();
}
