/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QThread>

#include "utils/robotCommunication/tcpRobotCommunicatorInterface.h"
#include "utils/utilsDeclSpec.h"

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicatorWorker;
enum class MessageKind;

/// Implementation of a class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public TcpRobotCommunicatorInterface
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param serverIpSettingsKey - where to find server ip setting in a registry.
	explicit TcpRobotCommunicator(const QString &serverIpSettingsKey);

	~TcpRobotCommunicator() override;

	void uploadProgram(const QString &programName) override;

	void runProgram(const QString &programName) override;

	void runDirectCommand(const QString &directCommand, bool asScript = false) override;

	void requestCasingVersion() override;

	void stopRobot() override;

	void requestData(const QString &sensor) override;

	void requestData() override;

	void connect() override;

	void disconnect() override;

private slots:
	/// Processes message from robot --- classifies it as info, error or text from stdout.
	void onMessageFromRobot(const MessageKind &messageKind, const QString &message);

	/// Handles connection error.
	void onConnectionError(const QString &error);

	/// Reports successful connection.
	void onConnected();

private:
	/// Worker object that handles all robot communication in separate thread.
	QScopedPointer<TcpRobotCommunicatorWorker> mWorker;

	/// Worker thread.
	QThread mWorkerThread;
};

}
}
