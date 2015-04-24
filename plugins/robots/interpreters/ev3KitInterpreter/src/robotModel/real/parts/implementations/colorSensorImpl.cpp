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

const unsigned colorSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

ColorSensorImpl::ColorSensorImpl(const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, SensorTypeEnum lowLevelType)
	: mImplementation(robotCommunicator, port, lowLevelType, READY_RAW)
	, mRobotCommunicator(robotCommunicator)
	, mLowLevelType(lowLevelType)
{
}

void ColorSensorImpl::read()
{
	const int size = 21;
	QByteArray command(size, 0);
	command[0] = size - 2;
	command[1] = size >> 8;
	command[2] = 4;
	command[3] = 0;
	command[4] = DIRECT_COMMAND_REPLY;
	const int globalVariablesCount = 4;
	const int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opINPUT_DEVICE;
	command[8] = READY_RAW;
	command[9] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[10] = 0x00; //layer
	command[11] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[12] = mImplementation.lowLevelPort(); //port[0-3]
	command[13] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[14] = 0x00; //type (0 = Don’t change type)
	command[15] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[16] = 0x00; //mode – Device mode [0-7]
	command[17] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[18] = 1; //Number of return values
	command[19] = 225;
	command[20] = 0;

	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);

	/// @todo Debug this.
	if (mLowLevelType == COLORFULL)
		emit newData(1);
		//emit newData(int(outputBuf.data()[5]));
	else if (static_cast<int>(outputBuf.data()[6]) < 20) {
		emit newData(1);
	} else {
		emit newData(1);
	}
}
