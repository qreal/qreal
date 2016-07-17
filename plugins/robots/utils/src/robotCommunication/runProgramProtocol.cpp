/* Copyright 2016 Yurii Litvinov, CyberTech Labs Ltd.
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
#include <QtCore/QCoreApplication>

#include "utils/robotCommunication/tcpRobotCommunicatorInterface.h"
#include "src/robotCommunication/protocol.h"

using namespace utils::robotCommunication;

RunProgramProtocol::RunProgramProtocol(TcpRobotCommunicatorInterface &communicator, const QString &configVersion)
	: mProtocol(new Protocol(communicator))
	, mWaitingForCasingModel(new QState())
	, mWaitingForUploadingComplete(new QState())
	, mWaitingForRunComplete(new QState())
{
	mProtocol->addCheckedTranstion(mWaitingForCasingModel, &TcpRobotCommunicatorInterface::casingVersionReceived
			, mWaitingForUploadingComplete
			, [this, configVersion](const QString &casingModel)
				{
					if (casingModel != configVersion) {
						emit configVersionMismatch(configVersion, casingModel);
						return false;
					}

					return true;
				}
	);

	mProtocol->addTransition(mWaitingForUploadingComplete
			, &TcpRobotCommunicatorInterface::uploadProgramDone, mWaitingForRunComplete);
	mProtocol->addErrorTransition(mWaitingForUploadingComplete, &TcpRobotCommunicatorInterface::uploadProgramError);
	mProtocol->addSuccessTransition(mWaitingForRunComplete, &TcpRobotCommunicatorInterface::startedRunning);

	connect(mProtocol.data(), &Protocol::success, this, &RunProgramProtocol::success);
	connect(mProtocol.data(), &Protocol::error, this, &RunProgramProtocol::error);
	connect(mProtocol.data(), &Protocol::timeout, this, &RunProgramProtocol::timeout);
}

RunProgramProtocol::~RunProgramProtocol()
{
}

void RunProgramProtocol::run(const QFileInfo &fileToRun)
{
	mProtocol->setAction(mWaitingForCasingModel, [](TcpRobotCommunicatorInterface &communicator) {
		communicator.requestCasingVersion();
	});

	mProtocol->setAction(mWaitingForUploadingComplete, [this, fileToRun](TcpRobotCommunicatorInterface &communicator) {
		communicator.uploadProgram(fileToRun.canonicalFilePath());
	});

	mProtocol->setAction(mWaitingForRunComplete, [this, fileToRun](TcpRobotCommunicatorInterface &communicator) {
		communicator.runProgram(fileToRun.fileName());
	});

	mProtocol->run();
}
