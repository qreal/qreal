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

#include "utils/robotCommunication/uploadProgramProtocol.h"

#include <QtCore/QStateMachine>
#include <QtCore/QState>
#include <QtCore/QFinalState>
#include <QtCore/QTimer>
#include <QtCore/QFileInfo>

#include "utils/robotCommunication/tcpRobotCommunicator.h"

#include <QtCore/QDebug>

using namespace utils::robotCommunication;

UploadProgramProtocol::UploadProgramProtocol(TcpRobotCommunicator &communicator)
	: mCommunicator(communicator)
	, mStateMachine(new QStateMachine())
	, mWaitingForUploadingComplete(new QState())
{
	const auto done = new QFinalState();
	const auto errored = new QFinalState();

	mTimeoutTimer.reset(new QTimer());
	mTimeoutTimer->setInterval(4000);
	mTimeoutTimer->setSingleShot(true);

	connect(mTimeoutTimer.data(), &QTimer::timeout, this, &UploadProgramProtocol::onTimeout);

	mWaitingForUploadingComplete->addTransition(&mCommunicator, &utils::TcpRobotCommunicator::uploadProgramDone
			, done);
	mWaitingForUploadingComplete->addTransition(&mCommunicator, &utils::TcpRobotCommunicator::connectionError
			, errored);
	mWaitingForUploadingComplete->addTransition(&mCommunicator, &utils::TcpRobotCommunicator::uploadProgramError
			, errored);

	connect(done, &QState::entered, [this]() {
		emit success();
		mTimeoutTimer->stop();
	});

	connect(errored, &QState::entered, [this]() {
		emit error();
		mTimeoutTimer->stop();
	});

	mStateMachine->addState(mWaitingForUploadingComplete);
	mStateMachine->addState(done);
	mStateMachine->addState(errored);

	mStateMachine->setInitialState(mWaitingForUploadingComplete);
}

UploadProgramProtocol::~UploadProgramProtocol()
{
}

void UploadProgramProtocol::run(const QFileInfo &fileToRun)
{
	if (mStateMachine->isRunning()) {
		return;
	}

	mWaitingForUploadingComplete->disconnect();

	connect(mWaitingForUploadingComplete, &QState::entered, [this, fileToRun]() {
		mCommunicator.uploadProgram(fileToRun.canonicalFilePath());
	});

	mStateMachine->start();
	mTimeoutTimer->start();
}

void UploadProgramProtocol::onTimeout()
{
	mStateMachine->stop();
	emit timeout();
}
