/* Copyright 2007-2015 QReal Research Group
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

#include "ev3RobotCommunicationThread.h"

class QTimer;
class libusb_device_handle;

namespace ev3 {
namespace communication {

class UsbRobotCommunicationThread : public Ev3RobotCommunicationThread
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

public slots:
	void send(QObject *addressee, const QByteArray &buffer, unsigned responseSize);
	void connect();
	void reconnect();
	void disconnect();

	void allowLongJobs(bool allow = true);
	void checkConsistency();

private slots:
	/// Checks if robot is connected
	void checkForConnection();

private:
	void send(const QByteArray &buffer, unsigned responseSize, QByteArray &outputBuffer) override;
	void send(const QByteArray &buffer) const override;

	QByteArray receive(int size) const override;

	libusb_device_handle *mHandle;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
};

}
}
