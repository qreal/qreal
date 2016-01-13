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

#include <ev3Kit/communication/commandConstants.h>
#include <ev3Kit/communication/ev3DirectCommand.h>

using namespace ev3::robotModel::real::parts;
using namespace ev3::communication;
using namespace kitBase::robotModel;
using namespace utils;

Ev3InputDevice::Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
		, const kitBase::robotModel::PortInfo &port)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
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

QByteArray Ev3InputDevice::readySiCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_SI, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index);        //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index);        // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}

QByteArray Ev3InputDevice::readyRawCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index);        // type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index);        // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}

QByteArray Ev3InputDevice::readyPercentCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_PCT, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index);        // type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index);        // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}
