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

#include "touchSensor.h"

const unsigned touchSensorResponseSize = 9;
const unsigned pressed = 63;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

TouchSensor::TouchSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::TouchSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void TouchSensor::read()
{
	const QByteArray command = mImplementation.readySiCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, touchSensorResponseSize, outputBuf);
	if (outputBuf.data()[8] == pressed) {
		emit newData(1);
	} else {
		emit newData(0);
	}
}
