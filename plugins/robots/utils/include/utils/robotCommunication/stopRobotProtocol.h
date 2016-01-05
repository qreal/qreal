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

class ROBOTS_UTILS_EXPORT StopRobotProtocol : public QObject
{
	Q_OBJECT

public:
	explicit StopRobotProtocol(TcpRobotCommunicator &communicator);
	~StopRobotProtocol() override;

	void run(const QString &command);

signals:
	void success();
	void error();
	void timeout();

private slots:
	void onTimeout();

private:
	QScopedPointer<Protocol> mProtocol;

	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForStopRobotCommandSent = nullptr;

	/// Does not have direct ownership (will be disposed by mProtocol).
	QState *mWaitingForDeinitializeCommandSent = nullptr;
};

}
}
