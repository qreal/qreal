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

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include "utils/utilsDeclSpec.h"

class QState;

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicator;
class Protocol;

/// Protocol for stopping robot and killing programs that may be launched by executing script. Uses state machine to do
/// it asynchronously and track protocol phase.
class ROBOTS_UTILS_EXPORT StopRobotProtocol : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param communicator - network communicator over which protocol will work.
	explicit StopRobotProtocol(TcpRobotCommunicator &communicator);

	~StopRobotProtocol() override;

	/// Stop robot and then execute "shutdownCommand" on it ("killall espeak", for example).
	void run(const QString &shutdownCommand);

signals:
	/// Emitted when protocol completed successfully.
	void success();

	/// Emitted when protocol completed with error.
	void error();

	/// Emitted when protocol finished by timeout. Most likely it means internal error in protocol state machine
	/// or in communicator, since network timeouts will be reported as error() signal.
	void timeout();

private:
	/// Underlying abstract protocol.
	QScopedPointer<Protocol> mProtocol;

	/// First phase of a protocol --- sending "stop" command.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForStopRobotCommandSent = nullptr;

	/// Second phase of a protocol --- executing shutdown command as direct command on robot.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForDeinitializeCommandSent = nullptr;
};

}
}
