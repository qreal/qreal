/* Copyright 2016 Yurii Litvinov
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

#include "utils/robotCommunication/runProgramProtocol.h"

#include <QtCore/QState>
#include <QtCore/QFileInfo>

#include "utils/robotCommunication/tcpRobotCommunicator.h"
#include "src/robotCommunication/protocol.h"

using namespace utils::robotCommunication;

RunProgramProtocol::RunProgramProtocol(TcpRobotCommunicator &communicator)
	: mProtocol(new Protocol(communicator))
	, mWaitingForUploadingComplete(new QState())
	, mWaitingForRunComplete(new QState())
{
	mProtocol->addTransition(mWaitingForUploadingComplete
			, &TcpRobotCommunicator::uploadProgramDone, mWaitingForRunComplete);
	mProtocol->addErrorTransition(mWaitingForUploadingComplete, &TcpRobotCommunicator::uploadProgramError);
	mProtocol->addSuccessTransition(mWaitingForRunComplete, &TcpRobotCommunicator::startedRunning);

	connect(mProtocol.data(), &Protocol::success, this, &RunProgramProtocol::success);
	connect(mProtocol.data(), &Protocol::error, this, &RunProgramProtocol::error);
	connect(mProtocol.data(), &Protocol::timeout, this, &RunProgramProtocol::timeout);
}

RunProgramProtocol::~RunProgramProtocol()
{
}

void RunProgramProtocol::run(const QFileInfo &fileToRun)
{
	mProtocol->setAction(mWaitingForUploadingComplete, [this, fileToRun](auto &communicator) {
		communicator.uploadProgram(fileToRun.canonicalFilePath());
	});

	mProtocol->setAction(mWaitingForRunComplete, [this, fileToRun](auto &communicator) {
		communicator.runProgram(fileToRun.fileName());
	});

	mProtocol->run();
}
