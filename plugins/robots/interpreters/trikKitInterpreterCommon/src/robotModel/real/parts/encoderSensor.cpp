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

#include "trikKitInterpreterCommon/robotModel/real/parts/encoderSensor.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator)
	: kitBase::robotModel::robotParts::EncoderSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::robotCommunication::TcpRobotCommunicator::newScalarSensorData
			, this, &EncoderSensor::onIncomingData);
}

void EncoderSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void EncoderSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

void EncoderSensor::nullify()
{
	const QString pathToCommand = ":/trikQts/templates/engines/nullifyEncoder.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@PORT@@", port().name());
	mRobotCommunicator.runDirectCommand(directCommand);
}
