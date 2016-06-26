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

#include "rangeSensor.h"

#include <qrkernel/logging.h>

const unsigned rangeSensorResponseSize = 10;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

RangeSensor::RangeSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::RangeSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void RangeSensor::read()
{
	QByteArray command = mImplementation.readySiCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, rangeSensorResponseSize, outputBuf);

	union {
		float f;
		uchar b[4];
	} floatFromBytesCast;
	floatFromBytesCast.b[3] = outputBuf.data()[8];
	floatFromBytesCast.b[2] = outputBuf.data()[7];
	floatFromBytesCast.b[1] = outputBuf.data()[6];
	floatFromBytesCast.b[0] = outputBuf.data()[5];

	const int data = qIsNaN(floatFromBytesCast.f) ? 0 : static_cast<int>(floatFromBytesCast.f);
	emit newData(data);
}
