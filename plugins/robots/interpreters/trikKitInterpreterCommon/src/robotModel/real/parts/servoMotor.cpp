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

#include "trikKitInterpreterCommon/robotModel/real/parts/servoMotor.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

ServoMotor::ServoMotor(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikServoMotor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void ServoMotor::on(int speed)
{
	const QString pathToCommand = ":/trikQts/templates/engines/forward.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@PORT@@", "\"" + port().name() + "\"")
			.replace("@@POWER@@", QString::number(speed)) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void ServoMotor::stop()
{
	on(0);
}

void ServoMotor::off()
{
	/// @todo It shall be a separate command, to power off motor, not to leave it in blocked state.
	on(0);
}
