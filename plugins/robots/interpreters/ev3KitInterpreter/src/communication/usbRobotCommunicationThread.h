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

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "robotCommunicationThreadBase.h"
//#include "fantom.h"

class QextSerialPort;

namespace nxtKitInterpreter {
namespace communication {

class UsbRobotCommunicationThread : public RobotCommunicationThreadBase
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect();
	void reconnect();
	void disconnect();
	void allowLongJobs(bool allow = true);
	void checkConsistency();

private slots:
	/// Checks if robot is connected
	void checkForConnection();

	/// Checks that message requires response or not.
	/// @returns true, if there shall be a response.
	static bool isResponseNeeded(QByteArray const &buffer);

private:
	static const int kStatusNoError = 0;

	bool isOpen();
	static void debugPrint(QByteArray const &buffer, bool out);

	void send(QByteArray const &buffer, unsigned const responseSize
			, QByteArray &outputBuffer);

	bool mActive;
	unsigned long mNXTHandle;
    //Fantom mFantom;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;

	bool mStopped;
};

}
}
