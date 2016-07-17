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

#include "lightSensor.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

const int maxLightValue = 1023;

LightSensor::LightSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::LightSensor(info, port)
	, mImplementation(robotCommunicator, port
			, enums::lowLevelSensorType::LIGHT_ACTIVE, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &LightSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &LightSensor::configurationCompleted);
}

void LightSensor::read()
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

void LightSensor::doConfiguration()
{
	mImplementation.configure();
}

void LightSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	mImplementation.setState(NxtInputDevice::idle);
	const int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	emit newData(sensorValue * 100 / maxLightValue);
}
