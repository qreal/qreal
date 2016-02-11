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

class TcpRobotCommunicator;
class Protocol;

/// Protocol for uploading program on a robot. Uses state machine to do it asynchronously.
class ROBOTS_UTILS_EXPORT UploadProgramProtocol : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param communicator - network communicator over which protocol will work.
	explicit UploadProgramProtocol(TcpRobotCommunicator &communicator);

	~UploadProgramProtocol() override;

	/// Upload program from given file to a robot.
	void run(const QFileInfo &programToUpload);

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

	/// The only phase of this simple protocol --- uploading program.
	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForUploadingComplete = nullptr;
};

}
}
