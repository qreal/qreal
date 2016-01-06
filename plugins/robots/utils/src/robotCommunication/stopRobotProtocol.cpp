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

#include "utils/robotCommunication/stopRobotProtocol.h"

#include <QtCore/QState>

#include "utils/robotCommunication/tcpRobotCommunicator.h"
#include "protocol.h"

using namespace utils::robotCommunication;

StopRobotProtocol::StopRobotProtocol(TcpRobotCommunicator &communicator)
	: mProtocol(new Protocol(communicator))
	, mWaitingForStopRobotCommandSent(new QState())
	, mWaitingForDeinitializeCommandSent(new QState())
{
	mProtocol->addTransition(mWaitingForStopRobotCommandSent, &TcpRobotCommunicator::stopRobotDone
			, mWaitingForDeinitializeCommandSent);
	mProtocol->addSuccessTransition(mWaitingForDeinitializeCommandSent, &TcpRobotCommunicator::runDirectCommandDone);

	connect(mProtocol.data(), &Protocol::success, this, &StopRobotProtocol::success);
	connect(mProtocol.data(), &Protocol::error, this, &StopRobotProtocol::error);
	connect(mProtocol.data(), &Protocol::timeout, this, &StopRobotProtocol::timeout);
}

StopRobotProtocol::~StopRobotProtocol()
{
}

void StopRobotProtocol::run(const QString &shutdownCommand)
{
	mProtocol->setAction(mWaitingForStopRobotCommandSent, [this](auto &communicator) {
		communicator.stopRobot();
	});

	mProtocol->setAction(mWaitingForDeinitializeCommandSent, [this, shutdownCommand](auto &communicator) {
		communicator.runDirectCommand(shutdownCommand, true);
	});

	mProtocol->run();
}
