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

#include "trikKitInterpreterCommon/robotModel/real/parts/speaker.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikSpeaker(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Speaker::play(const QString &filePath)
{
	const QString pathToCommand = ":/trikQts/templates/playTone.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@FILENAME@@", filePath) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
