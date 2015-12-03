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

#include "colorSensorBlue.h"

#include <ev3Kit/communication/ev3DirectCommand.h>

const unsigned colorSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace ev3::communication;
using namespace kitBase::robotModel;

ColorSensorBlue::ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void ColorSensorBlue::read()
{
	const QByteArray command = mImplementation.readyPercentCommand(mImplementation.lowLevelPort(), 4);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf[5]));
}
