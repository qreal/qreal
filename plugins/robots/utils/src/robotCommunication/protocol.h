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

class Protocol : public QObject
{
	Q_OBJECT

public:
	explicit Protocol(TcpRobotCommunicator &communicator, int timeout = 4000);
	~Protocol() override;

	template <typename Func> void addTransition(QState *source, Func signal, QState *destination)
	{
		registerStateIfNeeded(source);
		registerStateIfNeeded(destination);
		source->addTransition(&mCommunicator, signal, destination);
	}

	template <typename Func> void addErrorTransition(QState *source, Func signal)
	{
		registerStateIfNeeded(source);
		source->addTransition(&mCommunicator, signal, mErrored);
	}

	template <typename Func> void addSuccessTransition(QState *source, Func signal)
	{
		registerStateIfNeeded(source);
		source->addTransition(&mCommunicator, signal, mSuccess);
	}

	void setAction(QState *state, const std::function<void(TcpRobotCommunicator &communicator)> &action);

	void run();

signals:
	void success();
	void error();
	void timeout();

private slots:
	void onTimeout();

private:
	void registerStateIfNeeded(QState * const state);

	/// Does not have direct ownership, will be disposed by mStateMachine.
	QFinalState *mSuccess = nullptr;

	/// Does not have direct ownership, will be disposed by mStateMachine.
	QFinalState *mErrored = nullptr;

	TcpRobotCommunicator &mCommunicator;
	QScopedPointer<QStateMachine> mStateMachine;

	QScopedPointer<QTimer> mTimeoutTimer;

	/// Does not have direct ownership, all states here will be disposed by mStateMachine.
	QSet<QState *> mKnownStates;
};

}
}
