/* Copyright 2007-2016 QReal Research Group, Dmitry Mordvinov, Yurii Litvinov
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

#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>

#include "tcpConnectionHandler.h"

namespace utils {
namespace robotCommunication {

/// Enum that codes kind of a message from robot --- text from stdout, information or error.
enum class MessageKind
{
	text
	, info
	, error
};

/// Class that handles connection to robot and sends commands to it. Supposed to be working in a separate thread.
class TcpRobotCommunicatorWorker : public QObject
{
	Q_OBJECT

public:

	explicit TcpRobotCommunicatorWorker(const QString &robotIpRegistryKey);

	~TcpRobotCommunicatorWorker() override;

	/// Used to perform initialization when object is in its own thread, to aviod moving all its fields to a thread.
	/// Must be called right after moveToThread().
	Q_INVOKABLE void init();

	/// Turns off timers in worker thread before destructor is called from main thread. Must be called before worker
	/// thread is stopped.
	Q_INVOKABLE void deinit();

	/// Uploads given program to a robot using "file" command.
	/// @param programName - name of a file, it will be saved on a robot in "scripts" folder using that name.
	/// @param programContents - contents of a program to be saved on a robot.
	Q_INVOKABLE void uploadProgram(const QString &programName, const QString &programContents);

	/// Sends a command to run previously uploaded file in a robot.
	Q_INVOKABLE void runProgram(const QString &programName);

	/// Sends a script to be executed directly, without a need for a file.
	/// @param asScript - if true, the state of a robot will be reset before command evalutation.
	Q_INVOKABLE void runDirectCommand(const QString &directCommand, bool asScript);

	/// Sends a command to remotely abort script execution and stop robot.
	Q_INVOKABLE void stopRobot();

	/// Requests telemetry data for given sensor.
	Q_INVOKABLE void requestData(const QString &sensor);

	/// Establishes connection.
	Q_INVOKABLE void connect();

	/// Disconnects from robot.
	Q_INVOKABLE void disconnect();

signals:
	/// Emitted when TCP socket with robot was opened or failed to open.
	void connected();

	/// Emitted each time when connection with robot was aborted.
	void disconnected();

	/// Emitted when new scalar telemetry data is available for given port.
	void newScalarSensorData(const QString &port, int data);

	/// Emitted when new vector telemetry data is available for given port.
	void newVectorSensorData(const QString &port, const QVector<int> &data);

	/// Emitted when a robot wants to print something to stdout.
	void printText(const QString &text);

	/// Emitted when a robot starts program execution.
	void startedRunning();

	/// Emitted when connection can not be established or there is some connection error.
	/// @param errorString - error message to be shown to user.
	void connectionError(const QString &errorString);

	/// Emitted if we didn't receive version of trikRuntime within 3 seconds.
	void trikRuntimeVersionGettingError();

	/// Emitted if trikRuntime version does not match version required by this version of TRIK Studio.
	void trikRuntimeVersionError();

	/// Emitted when new message arrives from robot.
	void messageFromRobot(const MessageKind &messageKind, const QString &message);

	/// Emitted when program uploading is finished.
	void uploadProgramDone();

	/// Emitted when "stop robot" command is sent.
	void stopRobotDone();

	/// Emitted when direct command is sent.
	void runDirectCommandDone();

private slots:
	void processControlMessage(const QString &message);
	void processTelemetryMessage(const QString &message);
	void versionTimeOut();

private:
	/// Sends message using message length protocol (message is in form "<data length in bytes>:<data>").
	void send(const QString &data, QTcpSocket &socket);

	/// Sends version request and starts version timer
	void versionRequest();

	/// Name of a registry key where robot IP addres is stored. Stored here instead of actual IP since IP may change
	/// during the work and it is easier to get it from registry every time.
	const QString mRobotIpRegistryKey;

	QString mCurrentIp;
	QScopedPointer<TcpConnectionHandler> mControlConnection;
	QScopedPointer<TcpConnectionHandler> mTelemetryConnection;

	bool mIsConnected;

	/// Timer for version request.
	QScopedPointer<QTimer> mVersionTimer;
};

}
}
