/* Copyright 2015 CyberTech Labs Ltd.
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

#include <ev3Kit/communication/ev3DirectCommand.h>

const unsigned encoderSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::EncoderSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void EncoderSensor::read()
{
	QByteArray command = mImplementation.readyRawCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, encoderSensorResponseSize, outputBuf);
	int secondByte = static_cast<quint8>(outputBuf[6]) << 8;
	if (static_cast<int>(outputBuf[7]) < 0) {
		secondByte = static_cast<int>(outputBuf[6]) << 8;
	}

	emit newData(static_cast<quint8>(outputBuf[5]) | secondByte);
}

void EncoderSensor::nullify()
{
	QByteArray command = communication::Ev3DirectCommand::formCommand(12, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	communication::Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_CLR_COUNT, command, index);
	communication::Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	communication::Ev3DirectCommand::addByteParameter(lowLevelPort(), command, index);

	mRobotCommunicator.send(this, command, 3);
}

qint8 EncoderSensor::lowLevelPort() const
{
	return 1 << (this->port().name().toLower()[0].toLatin1() - 'a');
}
