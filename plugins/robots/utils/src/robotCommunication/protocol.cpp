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

#include "protocol.h"

#include <QtCore/QStateMachine>
#include <QtCore/QTimer>

#include "utils/robotCommunication/tcpRobotCommunicator.h"

using namespace utils::robotCommunication;

Protocol::Protocol(TcpRobotCommunicator &communicator, int timeout)
	: mSuccess(new QFinalState())
	, mErrored(new QFinalState())
	, mCommunicator(communicator)
	, mStateMachine(new QStateMachine())
{
	mTimeoutTimer.reset(new QTimer());
	mTimeoutTimer->setInterval(timeout);
	mTimeoutTimer->setSingleShot(true);

	connect(mTimeoutTimer.data(), &QTimer::timeout, this, &Protocol::onTimeout);

	connect(mSuccess, &QState::entered, [this]() {
		emit success();
		mTimeoutTimer->stop();
	});

	connect(mErrored, &QState::entered, [this]() {
		emit error();
		mTimeoutTimer->stop();
	});

	mStateMachine->addState(mSuccess);
	mStateMachine->addState(mErrored);
}

Protocol::~Protocol()
{
}

void Protocol::setAction(QState *state, const std::function<void(TcpRobotCommunicator &communicator)> &action)
{
	state->disconnect();
	connect(state, &QState::entered, [this, action](){ action(mCommunicator); });
}

void Protocol::run()
{
	if (mStateMachine->isRunning()) {
		return;
	}

	mStateMachine->start();
	mTimeoutTimer->start();
}

void Protocol::onTimeout()
{
	mStateMachine->stop();
	emit timeout();
}

void Protocol::registerStateIfNeeded(QState * const state)
{
	if (!mKnownStates.contains(state)) {
		mStateMachine->addState(state);
		if (mKnownStates.isEmpty()) {
			mStateMachine->setInitialState(state);
		}

		mKnownStates.insert(state);
		addErrorTransition(state, &TcpRobotCommunicator::connectionError);
	}
}
