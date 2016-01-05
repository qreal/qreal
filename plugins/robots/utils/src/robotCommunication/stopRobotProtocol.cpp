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

#include <QtCore/QStateMachine>
#include <QtCore/QState>
#include <QtCore/QFinalState>
#include <QtCore/QTimer>

#include "utils/robotCommunication/tcpRobotCommunicator.h"

using namespace utils::robotCommunication;

StopRobotProtocol::StopRobotProtocol(TcpRobotCommunicator &communicator)
	: mCommunicator(communicator)
	, mStateMachine(new QStateMachine())
	, mWaitingForStopRobotCommandSent(new QState())
	, mWaitingForDeinitializeCommandSent(new QState())
{
	const auto done = new QFinalState();
	const auto errored = new QFinalState();

	mTimeoutTimer.reset(new QTimer());
	mTimeoutTimer->setInterval(4000);
	mTimeoutTimer->setSingleShot(true);

	connect(mTimeoutTimer.data(), &QTimer::timeout, this, &StopRobotProtocol::onTimeout);

	mWaitingForStopRobotCommandSent->addTransition(&mCommunicator, &TcpRobotCommunicator::stopRobotDone
			, mWaitingForDeinitializeCommandSent);
	mWaitingForStopRobotCommandSent->addTransition(&mCommunicator, &TcpRobotCommunicator::connectionError
			, errored);

	mWaitingForDeinitializeCommandSent->addTransition(&mCommunicator, &TcpRobotCommunicator::runDirectCommandDone
			, done);
	mWaitingForDeinitializeCommandSent->addTransition(&mCommunicator, &TcpRobotCommunicator::connectionError
			, errored);

	connect(done, &QState::entered, [this]() {
		emit success();
		mTimeoutTimer->stop();
	});

	connect(errored, &QState::entered, [this]() {
		emit error();
		mTimeoutTimer->stop();
	});

	mStateMachine->addState(mWaitingForStopRobotCommandSent);
	mStateMachine->addState(mWaitingForDeinitializeCommandSent);
	mStateMachine->addState(done);
	mStateMachine->addState(errored);

	mStateMachine->setInitialState(mWaitingForStopRobotCommandSent);
}

StopRobotProtocol::~StopRobotProtocol()
{
}

void StopRobotProtocol::run(const QString &command)
{
	if (mStateMachine->isRunning()) {
		return;
	}

	mWaitingForStopRobotCommandSent->disconnect();
	mWaitingForDeinitializeCommandSent->disconnect();

	connect(mWaitingForStopRobotCommandSent, &QState::entered, [this]() {
		mCommunicator.stopRobot();
	});

	connect(mWaitingForDeinitializeCommandSent, &QState::entered, [this, command]() {
		mCommunicator.runDirectCommand(command, true);
	});

	mStateMachine->start();
	mTimeoutTimer->start();
}

void StopRobotProtocol::onTimeout()
{
	mStateMachine->stop();
	emit timeout();
}
