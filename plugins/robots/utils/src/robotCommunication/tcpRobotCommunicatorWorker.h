/* Copyright 2013-2016 CyberTech Labs Ltd., Dmitry Mordvinov, Yurii Litvinov, Grigorii Zimin
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
	, fileContents
	, mail
	, info
	, error
};

/// Class that handles connection to robot and sends commands to it. Supposed to be working in a separate thread.
class TcpRobotCommunicatorWorker : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param robotIpRegistryKey - key in a registry where current robot IP address is stored. IP address refreshes
	///        from registry every time new connection needs to be established.
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

	/// Requests casing version from robot, emits casingVersionReceived() when robot responds.
	Q_INVOKABLE void requestCasingVersion();

	/// Requests telemetry data for given sensor.
	Q_INVOKABLE void requestData(const QString &sensor);

	/// Requests telemetry data for all ports.
	Q_INVOKABLE void requestData();

	/// Establishes connection.
	Q_INVOKABLE void connect();

	/// Disconnects from robot.
	Q_INVOKABLE void disconnectConnection();

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

	/// Emitted when received TRIK casing version (model 2014 or model 2015) from robot.
	void casingVersionReceived(const QString &casingVersion);

private slots:
	/// Process message from control connection, emits signals when something interesting is received from robot.
	void processControlMessage(const QString &message);

	/// Process telemetry message from robot. Emits signals with sensor data.
	void processTelemetryMessage(const QString &message);

	/// TRIK Runtime version request timed out. Most likely caused by network problems.
	void onVersionTimeOut();

private:
	/// Handles value from telemetry message from robot. Emits signals with sensor data.
	void handleValue(const QString &data);

	/// Sends message using message length protocol (message is in form "<data length in bytes>:<data>").
	void send(const QString &data, QTcpSocket &socket);

	/// Sends version request and starts version timer
	void versionRequest();

	/// Name of a registry key where robot IP addres is stored. Stored here instead of actual IP since IP may change
	/// during the work and it is easier to get it from registry every time.
	const QString mRobotIpRegistryKey;

	/// Current IP address of a robot.
	QString mCurrentIp;

	/// Connection on which all commands are sent to a robot.
	QScopedPointer<TcpConnectionHandler> mControlConnection;

	/// Connection on which robot sends telemetry data.
	QScopedPointer<TcpConnectionHandler> mTelemetryConnection;

	/// Timer for version request.
	QScopedPointer<QTimer> mVersionTimer;
};

}
}
