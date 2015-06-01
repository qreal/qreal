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

#include "colorSensorFull.h"

const unsigned colorSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

ColorSensorFull::ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
						 , const kitBase::robotModel::PortInfo &port
						 , utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorFull(info, port)
	, mColorSensor(port, robotCommunicator, enums::sensorType::SensorTypeEnum::COLOR_FULL)
	, mRobotCommunicator(robotCommunicator)
{
}

void ColorSensorFull::read()
{
	//Something weird with ColorSensorFull and ColorSensorImpl
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(0, command, index); //port
	Ev3DirectCommand::addByteParameter(0x00, command, index); //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(2, command, index); //mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index); // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index); // index for return data

	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[6]));
}

