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

#include "led.h"

#include <QtCore/QDebug>

#include <ev3Kit/communication/commandConstants.h>
#include <ev3Kit/communication/ev3DirectCommand.h>

using namespace ev3::robotModel::real::parts;
using namespace ev3::communication;
using namespace kitBase::robotModel;

Led::Led(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotModel::parts::Ev3Led(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Led::setColor(ev3::robotModel::parts::Ev3LedColor color)
{
	QByteArray command = Ev3DirectCommand::formCommand(11, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_WRITE_LED, command, index);
	Ev3DirectCommand::addByteParameter(colorByte(color), command, index);

	mRobotCommunicator.send(this, command, 3);
}

qint8 Led::colorByte(ev3::robotModel::parts::Ev3LedColor color) const
{
	switch (color) {
	case ev3::robotModel::parts::Ev3LedColor::off:
		return 0;
	case ev3::robotModel::parts::Ev3LedColor::green:
		return 1;
	case ev3::robotModel::parts::Ev3LedColor::red:
		return 2;
	case ev3::robotModel::parts::Ev3LedColor::orange:
		return 3;
	case ev3::robotModel::parts::Ev3LedColor::greenFlash:
		return 4;
	case ev3::robotModel::parts::Ev3LedColor::redFlash:
		return 5;
	case ev3::robotModel::parts::Ev3LedColor::orangeFlash:
		return 6;
	case ev3::robotModel::parts::Ev3LedColor::greenPulse:
		return 7;
	case ev3::robotModel::parts::Ev3LedColor::redPulse:
		return 8;
	case ev3::robotModel::parts::Ev3LedColor::orangePulse:
		return 9;
	default:
		qWarning() << "Unknown EV3 LED color:" << static_cast<int>(color);
		return 0;
	}
}
