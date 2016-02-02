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

#include "soundSensor.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

const int soundMaxValue = 1023;

SoundSensor::SoundSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::SoundSensor(info, port)
	, mImplementation(robotCommunicator, port, enums::lowLevelSensorType::SOUND_DBA, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &SoundSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &SoundSensor::configurationCompleted);
}

void SoundSensor::read()
{
	if (!mImplementation.isConfigured()) {
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

void SoundSensor::doConfiguration()
{
	mImplementation.configure();
}

void SoundSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		/// @todo: log trace error?
	} else {
		const int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
		mImplementation.setState(NxtInputDevice::idle);
		emit newData(sensorValue * 100 / soundMaxValue);
	}
}
