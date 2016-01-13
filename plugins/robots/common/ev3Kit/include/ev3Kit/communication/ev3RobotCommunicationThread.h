/* Copyright 2015 CyberTech Labs Ltd.
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

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>

namespace ev3 {
namespace communication {

/// Interface for EV3 communicators. Must be able to upload, start and stop robots to EV3 brick.
class Ev3RobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	virtual ~Ev3RobotCommunicationThread();

public slots:
	/// Uploads file on the local machine to a remote device via Bluetooth.
	/// @returns path to uploaded file on EV3 brick if it was uploaded successfully or empty string otherwise.
	virtual QString uploadFile(const QString &sourceFile, const QString &targetDir);

	/// Starts program execution on EV3 brick. Does not upload the program itself.
	virtual bool runProgram(const QString &pathOnRobot = QString());

	/// Stops currently executing program in EV3 brick.
	virtual void stopProgram();

protected:
	/// Must be reimplemented in each thread just to send the given buffer.
	virtual void send(const QByteArray &buffer) const = 0;

	/// Must be reimplemented in each thread just to recieve the buffer of the given size.
	virtual QByteArray receive(int size) const = 0;
};

}
}
