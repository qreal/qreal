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

#include "encoderSensor.h"

#include "commandConstants.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::EncoderSensor(info, port)
	, mRobotCommunicator(robotCommunicator)
	, mState(idle)
{
	connect(&mRobotCommunicator, &utils::robotCommunication::RobotCommunicator::response
			, this, &EncoderSensor::readingDone);
}

void EncoderSensor::read()
{
	if (mState == pending) {
		return;
	}

	mState = pending;
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETOUTPUTSTATE;
	command[4] = lowLevelPort();
	mRobotCommunicator.send(this, command, 27);
}

void EncoderSensor::nullify()
{
	QByteArray command(6, 0);
	command[0] = 0x04;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::RESETMOTORPOSITION;
	command[4] = lowLevelPort();
	command[5] = false;
	mRobotCommunicator.send(this, command, 0);
}

void EncoderSensor::readingDone(QObject *addressee, const QByteArray &reading)
{
	if (addressee != this) {
		return;
	}

	sensorSpecificProcessResponse(reading);
}

void EncoderSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	mState = idle;

	if (reading.isEmpty()) {
		/// @todo: log trace error?
	} else {
		unsigned int recieved = (0xff & reading[23]) | ((0xff & reading[24]) << 8)
				| ((0xff & reading[25]) << 16) | ((0xff & reading[26]) << 24);
		emit newData(recieved);
	}
}

char EncoderSensor::lowLevelPort() const
{
	return port().name().at(0).toLatin1() - 'A';
}
