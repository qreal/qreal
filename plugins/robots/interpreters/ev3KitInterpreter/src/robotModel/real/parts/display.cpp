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

#include "display.h"

#include <ev3Kit/communication/commandConstants.h>
#include <ev3Kit/communication/ev3DirectCommand.h>

using namespace ev3::robotModel::real::parts;
using namespace ev3::communication;
using namespace kitBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Display::Display(const DeviceInfo &info, const PortInfo &port, RobotCommunicator &robotCommunicator)
	: robotModel::parts::Ev3Display(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Display::drawPixel(int x, int y)
{
	QByteArray command = Ev3DirectCommand::formCommand(19, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::UI_DRAW_PIXEL, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	QByteArray command = Ev3DirectCommand::formCommand(25, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_LINE, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x1, command, index);
	Ev3DirectCommand::addShortParameter(y1, command, index);
	Ev3DirectCommand::addShortParameter(x2, command, index);
	Ev3DirectCommand::addShortParameter(y2, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height, bool filled)
{

	QByteArray command = Ev3DirectCommand::formCommand(25, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	if (filled) {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_RECT, command, index);
	} else {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_RECT, command, index);
	}
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addShortParameter(width, command, index);
	Ev3DirectCommand::addShortParameter(height, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawCircle(int x, int y, int radius, bool filled)
{
	QByteArray command = Ev3DirectCommand::formCommand(22, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	if (filled) {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_CIRCLE, command, index);
	} else {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CIRCLE, command, index);
	}
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addShortParameter(radius, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}


void Display::printText(int x, int y, const QString &text)
{
	QByteArray command = Ev3DirectCommand::formCommand(21 + text.length(), 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_TEXT, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addStringParameter(text, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::clearScreen()
{
	QByteArray command = Ev3DirectCommand::formCommand(11, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CLEAN, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::redraw()
{
}
