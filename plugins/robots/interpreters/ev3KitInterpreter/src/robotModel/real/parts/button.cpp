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

#include "button.h"

#include <ev3Kit/communication/commandConstants.h>
#include <ev3Kit/communication/ev3DirectCommand.h>

const unsigned buttonResponseSize = 6;
const unsigned buttonPressed = 0x01;

using namespace ev3::robotModel::real;
using namespace ev3::robotModel::real::parts;
using namespace ev3::communication;
using namespace kitBase::robotModel;

Button::Button(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::Button(info, port, -1)
	, mRobotCommunicator(robotCommunicator)
{
}

void Button::read()
{
	QByteArray command = Ev3DirectCommand::formCommand(13, 4, 1, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_BUTTON_PRESSED, command, index);
	Ev3DirectCommand::addByteParameter(parsePort(port().name()), command, index);
	Ev3DirectCommand::addGlobalIndex(0, command, index);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, buttonResponseSize, outputBuf);
	if (outputBuf.length() == buttonResponseSize && outputBuf.data()[5] == buttonPressed) {
		emit newData(1);
	} else {
		emit newData(0);
	}
}

char Button::parsePort(const QString &portName)
{
	if (portName == "UpButtonPort") {
		return enums::brickButton::BrickButtonEnum::UP;
	} else if (portName == "EnterButtonPort") {
		return enums::brickButton::BrickButtonEnum::ENTER;
	} else if (portName == "DownButtonPort") {
		return enums::brickButton::BrickButtonEnum::DOWN;
	} else if (portName == "RightButtonPort") {
		return enums::brickButton::BrickButtonEnum::RIGHT;
	} else if (portName == "LeftButtonPort") {
		return enums::brickButton::BrickButtonEnum::LEFT;
	} else if (portName == "BackButtonPort") {
		return enums::brickButton::BrickButtonEnum::BACK;
	}

	return enums::brickButton::BrickButtonEnum::ANY;
}
