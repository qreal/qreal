/* Copyright 2012-2016 QReal Research Group
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

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>

class libusb_device_handle;

namespace nxt {
namespace communication {

class NxtUsbDriverInstaller;

class UsbRobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

public slots:
	bool send(QObject *addressee, const QByteArray &buffer, int responseSize) override;
	bool connect() override;
	void reconnect() override;
	void disconnect() override;
	void allowLongJobs(bool allow = true) override;

	/// Searches for NXT brick in formware mode, connects to it, returns success of this operation.
	bool connectFirmware();

signals:
	/// Emitted when attempt to connect failed because no NXT drivers installer on the machine.
	void noDriversFound();

private slots:
	/// Checks if robot is connected
	void checkForConnection();

	/// Checks that message requires response or not.
	/// @returns true, if there shall be a response.
	static bool isResponseNeeded(const QByteArray &buffer);

private:
	static const int kStatusNoError = 0;

	bool connectImpl(bool firmwareMode, int pid, int vid, const QString &notConnectedErrorText);
	bool send(const QByteArray &buffer, int responseSize, QByteArray &outputBuffer) override;

	libusb_device_handle *mHandle;
	bool mFirmwareMode;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
	QScopedPointer<NxtUsbDriverInstaller> mDriverInstaller;

	bool mStopped;
};

}
}
