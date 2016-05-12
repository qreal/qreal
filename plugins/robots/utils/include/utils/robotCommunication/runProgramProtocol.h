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
class QFileInfo;

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicatorInterface;
class Protocol;

/// Protocol for uploading and running program on a robot. Uses state machine to do it asynchronously and track
/// protocol phase.
class ROBOTS_UTILS_EXPORT RunProgramProtocol : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param communicator - network communicator over which protocol will work.
	RunProgramProtocol(TcpRobotCommunicatorInterface &communicator, const QString &configVersion);

	~RunProgramProtocol() override;

	/// Upload and run program from given file on a robot.
	void run(const QFileInfo &fileToRun);

signals:
	/// Emitted when protocol completed successfully.
	void success();

	/// Emitted when protocol completed with error.
	void error();

	/// Emitted when protocol finished by timeout. Most likely it means internal error in protocol state machine
	/// or in communicator, since network timeouts will be reported as error() signal.
	void timeout();

	/// Emitted when version of configuration file on a robot does not match version required by selected robot model.
	/// @param expected --- version required by robot model selected in TRIK Studio.
	/// @param actual --- version reported by robot.
	void configVersionMismatch(const QString &expected, const QString &actual);

private:
	/// Underlying abstract protocol.
	QScopedPointer<Protocol> mProtocol;

	/// First phase of a protocol --- checking TRIK casing model.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForCasingModel = nullptr;

	/// Second phase of a protocol --- uploading program.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForUploadingComplete = nullptr;

	/// Third phase of a protocol --- sending "run program" command on robot.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForRunComplete = nullptr;
};

}
}
