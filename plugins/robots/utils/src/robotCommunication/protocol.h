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

#pragma once

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QState>
#include <QtCore/QFinalState>
#include <QtCore/QSet>

class QStateMachine;
class QTimer;

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicator;

/// Abstract state machine-based protocol of interaction with robot. Uses states to track phase of interaction,
/// transitions initiated by TcpRobotCommunicator signals to move between states and actions on entering in state
/// to do actual communication.
///
/// First registered state will be the starting state of a state machine. State machine has two internal final states:
/// "success" and "errored": when state machine moves to "sucess" state it emits "success()" signal, when in "errored"
/// --- "error()" signal, then in either case aborts execution. Every registered state will have transition to error
/// state on TcpRobotCommunicator::connectionError signal, more transitions to error state can be added manually.
class Protocol : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param communicator - network communicator over which protocol will work.
	/// @param timeout - safety timeout that aborts protocol execution. Since network timeouts will be properly reported
	///        by TcpRobotCommunicator as errors, hitting this timeout means that something is wrong in
	///        TcpRobotCommunicator or protocol itself (incorrect state machine, for example).
	explicit Protocol(TcpRobotCommunicator &communicator, int timeout = 4000);

	~Protocol() override;

	/// Adds transition from "source" state to "destination" state initiated by given signal.
	template <typename Func> void addTransition(QState *source, Func signal, QState *destination)
	{
		registerStateIfNeeded(source);
		registerStateIfNeeded(destination);
		source->addTransition(&mCommunicator, signal, destination);
	}

	/// Adds transition from "source" state to error state by given signal.
	template <typename Func> void addErrorTransition(QState *source, Func signal)
	{
		registerStateIfNeeded(source);
		source->addTransition(&mCommunicator, signal, mErrored);
	}

	/// Adds transition from "source" state to success state by given signal.
	template <typename Func> void addSuccessTransition(QState *source, Func signal)
	{
		registerStateIfNeeded(source);
		source->addTransition(&mCommunicator, signal, mSuccess);
	}

	/// Sets action that shall be done when entering a state. Action receives communicator to be able to send
	/// something over network.
	void setAction(QState *state, const std::function<void(TcpRobotCommunicator &communicator)> &action);

	/// Executes configured protocol asyncronously.
	void run();

signals:
	/// Emitted when protocol completed successfully.
	void success();

	/// Emitted when protocol completed with error.
	void error();

	/// Emitted when protocol finished by timeout. Most likely it means internal error in protocol state machine
	/// or in communicator, since network timeouts will be reported as error() signal.
	void timeout();

private slots:
	/// Called when protocol timeout reached.
	void onTimeout();

private:
	/// If this state is not added to state machine yet, adds it, adds transition to error state on network error
	/// and if this state is first in a protocol, marks it as starting state.
	void registerStateIfNeeded(QState * const state);

	/// Special internal state meaning successful protocol execution.
	/// Does not have direct ownership, will be disposed by mStateMachine.
	QFinalState *mSuccess = nullptr;

	/// Special internal state meaning that some errors occured during protocol execution and it finished
	/// unsuccessfully.
	/// Does not have direct ownership, will be disposed by mStateMachine.
	QFinalState *mErrored = nullptr;

	/// Network communicator over which protocol works.
	TcpRobotCommunicator &mCommunicator;

	/// State machine used to track protocol state.
	QScopedPointer<QStateMachine> mStateMachine;

	/// Timer for failsafe timeout. Aborts protocol execution if it is taking too long.
	QScopedPointer<QTimer> mTimeoutTimer;

	/// Set of states currently registered in state machine.
	/// Does not have direct ownership, all states here will be disposed by mStateMachine.
	QSet<QState *> mKnownStates;
};

}
}
