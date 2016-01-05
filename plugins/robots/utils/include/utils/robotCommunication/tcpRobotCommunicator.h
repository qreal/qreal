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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "utils/robotCommunication/tcpConnectionHandler.h"
#include "utils/utilsDeclSpec.h"

namespace utils {

class TcpRobotCommunicatorWorker;
enum class MessageKind;

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param serverIpSettingsKey - where to find server ip setting in a registry.
	explicit TcpRobotCommunicator(const QString &serverIpSettingsKey);

	~TcpRobotCommunicator() override;

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	void uploadProgram(const QString &programName);

	/// Sends a command to run previously uploaded file in a robot.
	void runProgram(const QString &programName);

	/// Sends a script to be executed directly, without a need for a file.
	/// @param asScript - if true, the state of a robot will be reset before command evalutation.
	void runDirectCommand(const QString &directCommand, bool asScript = false);

	/// Sends a command to remotely abort script execution and stop robot.
	void stopRobot();

	/// Requests telemetry data for given sensor.
	void requestData(const QString &sensor);

	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	void connect();

	/// Disconnects from robot.
	void disconnect();

	/// Starts to use the given \a errorReporter for diplaying errors and messages to user.
	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);

signals:
	/// Emitted when TCP socket with robot was opened or failed to open.
	void connected(bool success, const QString &errorMessage);

	/// Emitted each time when connection with robot was aborted.
	void disconnected();

	/// Emitted when new scalar telemetry data is available for given port.
	void newScalarSensorData(const QString &port, int data);

	/// Emitted when new vector telemetry data is available for given port.
	void newVectorSensorData(const QString &port, QVector<int> const &data);

	/// Emitted when a robot wants to print something to stdout.
	void printText(const QString &text);

	/// Emitted when a robot starts program execution.
	void startedRunning();

	void uploadProgramDone();

	void uploadProgramError();

	void runDirectCommandDone();

	void stopRobotDone();

	void connectionError(const QString &errorString);

private slots:
	void onMessageFromRobot(const MessageKind &messageKind, const QString &message);

	void onConnectionError(const QString &error);

	void onTrikRuntimeVersionGettingError();

	void onTrikRuntimeVersionError();

	void onConnected();

private:
	/// Worker object that handles all robot communication in separate thread.
	QScopedPointer<TcpRobotCommunicatorWorker> mWorker;

	/// Worker thread.
	QThread mWorkerThread;

	/// Reference to error reporter.
	qReal::ErrorReporterInterface *mErrorReporter = nullptr;  // Does not have ownership.
};

}
