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

#include "nxtInputDevice.h"

using namespace nxt::robotModel::real::parts;
using namespace utils;

NxtInputDevice::NxtInputDevice(
		utils::robotCommunication::RobotCommunicator &robotCommunicator
		, const kitBase::robotModel::PortInfo &port
		, const enums::lowLevelSensorType::SensorTypeEnum &lowLevelSensorType
		, const enums::sensorMode::SensorModeEnum &sensorMode)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
	, mState(idle)
	, mIsConfigured(false)
	, mResetDone(false)
{
	QObject::connect(&mRobotCommunicator, &utils::robotCommunication::RobotCommunicator::response
			, this, &NxtInputDevice::readingDone);
}

void NxtInputDevice::readingDone(QObject *addressee, const QByteArray &reading)
{
	if (addressee == this) {
		processResponse(reading);
	}
}

void NxtInputDevice::processResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		mState = idle;
		if (mResetDone) {
			// Just ignore connection failures for now
			// emit failure();
		}
	} else if (reading.size() >= 5 && reading[3] == enums::commandCode::RESETINPUTSCALEDVALUE) {
		mState = idle;
		mResetDone = true;
		emit configured(true);
	} else if (reading.size() >= 5 && reading[3] == enums::commandCode::SETINPUTMODE) {
		mState = idle;
		QByteArray command(5, 0);
		command[0] = 0x03;
		command[1] = 0x00;
		command[2] = enums::telegramType::directCommandResponseRequired;
		command[3] = enums::commandCode::RESETINPUTSCALEDVALUE;
		command[4] = mLowLevelPort;
		send(command, 5);

		mIsConfigured = true;
	} else {
		emit sensorSpecificProcessResponse(reading);
	}
}

void NxtInputDevice::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::SETINPUTMODE;
	command[4] = mLowLevelPort;
	command[5] = mLowLevelSensorType;
	command[6] = mSensorMode;
	send(command, 5);
}

void NxtInputDevice::send(const QByteArray &buffer, const unsigned responseSize)
{
	mRobotCommunicator.send(this, buffer, responseSize);
}

NxtInputDevice::State NxtInputDevice::state() const
{
	return mState;
}

void NxtInputDevice::setState(NxtInputDevice::State state)
{
	mState = state;
}

bool NxtInputDevice::isConfigured() const
{
	return mIsConfigured;
}

char NxtInputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}
