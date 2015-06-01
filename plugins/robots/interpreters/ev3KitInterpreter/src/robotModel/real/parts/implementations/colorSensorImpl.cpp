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

#include "colorSensorImpl.h"
#include "src/commandConstants.h"

const unsigned colorSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

ColorSensorImpl::ColorSensorImpl(const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, enums::sensorType::SensorTypeEnum lowLevelType)
	: mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
	, mLowLevelType(lowLevelType)
{
}

void ColorSensorImpl::read()
{
	QByteArray command = mImplementation.readyRawCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);

	/// @todo Debug this.
	if (mLowLevelType == enums::sensorType::SensorTypeEnum::COLOR_FULL)
		emit newData(1);
		//emit newData(int(outputBuf.data()[5]));
	else if (static_cast<int>(outputBuf.data()[6]) < 20) {
		emit newData(1);
	} else {
		emit newData(1);
	}
}
