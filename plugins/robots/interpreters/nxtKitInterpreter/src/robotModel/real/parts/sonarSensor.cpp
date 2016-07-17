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

#include "sonarSensor.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

SonarSensor::SonarSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)

	: kitBase::robotModel::robotParts::RangeSensor(info, port)
	, mImplementation(robotCommunicator, port
			, enums::lowLevelSensorType::LOWSPEED_9V, enums::sensorMode::RAWMODE)
	, mI2C(robotCommunicator)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &SonarSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured
			, this, &SonarSensor::configurationCompleted);
}

void SonarSensor::read()
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
	setMode(enums::sonarMode::SINGLE_SHOT);
}

void SonarSensor::doConfiguration()
{
	mImplementation.configure();
}

void SonarSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		/// @todo: log trace error?
	} else if (reading.size() == 1 && reading[0] == 0) {
		// Sensor configured, now we can send actual request.
		QByteArray command(2, 0);
		command[0] = enums::sonarRegisters::SONAR_ADDRESS;
		command[1] = enums::sonarRegisters::RESULT_1;
		sendCommand(command, 1);
	} else if (reading.size() == 1 && reading[0] != 0) {
		/// @todo: log trace error?
	} else {
		mImplementation.setState(NxtInputDevice::idle);
		emit newData(0xff & reading[1]);
	}
}

void SonarSensor::setMode(nxt::enums::sonarMode::SonarModeEnum mode)
{
	writeRegister(enums::sonarRegisters::COMMAND, mode);
}

void SonarSensor::writeRegister(nxt::enums::sonarRegisters::SonarRegistersEnum reg, int value)
{
	QByteArray command(3, 0);
	command[0] = enums::sonarRegisters::SONAR_ADDRESS;
	command[1] = reg;
	command[2] = value;

	sendCommand(command, 0);
}

void SonarSensor::sendCommand(const QByteArray &command, int responseSize)
{
	mI2C.sendI2C(&mImplementation, command, responseSize, port());
}
