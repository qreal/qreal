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

#include <QtCore/QObject>

#include "utils/utilsDeclSpec.h"

namespace utils {
namespace robotCommunication {

/// Interface for robot communication, needed for testing by creation of communicator mock.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicatorInterface : public QObject
{
	Q_OBJECT

public:
	/// Reads generated program from a file and uploads it to a robot using "file" command.
	virtual void uploadProgram(const QString &programName) = 0;

	/// Sends a command to run previously uploaded file in a robot.
	virtual void runProgram(const QString &programName) = 0;

	/// Sends a script to be executed directly, without a need for a file.
	/// @param asScript - if true, the state of a robot will be reset before command evalutation.
	virtual void runDirectCommand(const QString &directCommand, bool asScript = false) = 0;

	/// Requests casing version from robot, emits casingVersionReceived() when robot responds.
	virtual void requestCasingVersion() = 0;

	/// Sends a command to remotely abort script execution and stop robot.
	virtual void stopRobot() = 0;

	/// Requests telemetry data for given sensor.
	virtual void requestData(const QString &sensor) = 0;

	/// Requests telemetry data for all ports.
	virtual void requestData() = 0;

	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	virtual void connect() = 0;

	/// Disconnects from robot.
	virtual void disconnect() = 0;

signals:
	/// Emitted when TCP socket with robot was opened or failed to open.
	void connected(bool success, const QString &errorMessage);

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

	/// Emitted when program uploaded successfully.
	void uploadProgramDone();

	/// Emitted when program upload failed by reasons not related to network error.
	/// @param error - user-readable error message describing what went wrong.
	void uploadProgramError(const QString &error);

	/// Emitted when direct command sent successfully.
	void runDirectCommandDone();

	/// Emitted when "stop" command sent successfully.
	void stopRobotDone();

	/// Emitted when there was a network error.
	/// @param error - user-readable error message describing what went wrong.
	void connectionError(const QString &error);

	/// Emitted if we didn't receive version of trikRuntime within 3 seconds.
	void trikRuntimeVersionGettingError();

	/// Emitted if trikRuntime version does not match version required by this version of TRIK Studio.
	void trikRuntimeVersionError();

	/// Emitted when received TRIK casing version (model 2014 or model 2015) from robot.
	void casingVersionReceived(const QString &casingVersion);

	/// Emitted when new information message arrives from robot.
	void infoFromRobot(const QString &message);

	/// Emitted when new error message arrives from robot.
	void errorFromRobot(const QString &message);

	/// Emitted when new message with "file contents" arrives from robot.
	void fileContentsFromRobot(const QString &message);

	/// Emitted when new mail arrives from robot.
	void mailFromRobot(const QString &message);
};

}
}
