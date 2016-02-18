/* Copyright 2012-2015 QReal Research Group
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

class QextSerialPort;
class QTimer;

namespace nxt {
namespace communication {

class BluetoothRobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	BluetoothRobotCommunicationThread();
	~BluetoothRobotCommunicationThread();

public slots:
	bool send(QObject *addressee, const QByteArray &buffer, int responseSize) override;
	bool connect() override;
	void reconnect() override;
	void disconnect() override;

	void allowLongJobs(bool allow = true) override;

private slots:
	/// Checks if robot is connected
	void checkForConnection();

private:
	bool send(const QByteArray &buffer, int responseSize, QByteArray &outputBuffer) override;
	bool send(const QByteArray &buffer) const;
	QByteArray receive(int size) const;

	QextSerialPort *mPort;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
};

}
}
