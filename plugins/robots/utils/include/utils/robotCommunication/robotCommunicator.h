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

#include <QtCore/QString>
#include <QtCore/QThread>

#include "utils/utilsDeclSpec.h"
#include "robotCommunicationThreadInterface.h"

class QextSerialPort;

namespace utils {
namespace robotCommunication {

class ROBOTS_UTILS_EXPORT RobotCommunicator : public QObject
{
	Q_OBJECT

public:
	explicit RobotCommunicator(QObject *parent = 0);
	~RobotCommunicator();

	void send(QObject *addressee, const QByteArray &buffer, const unsigned responseSize);
	void send(const QByteArray &buffer, const unsigned responseSize, QByteArray &outputBuffer);
	void connect();
	void disconnect();

	/// Returns a pointer to communication thread object that currently implementing this communicator.
	RobotCommunicationThreadInterface *currentCommunicator() const;
	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

	/// Checks if connection can be established or emits errorOccured();
	void checkConsistency();

signals:
	void errorOccured(const QString &message);
	void connected(bool success, const QString &errorString);
	void disconnected();
	void response(QObject *addressee, const QByteArray &buffer);

private:
	QThread mRobotCommunicationThread;
	RobotCommunicationThreadInterface *mRobotCommunicationThreadObject;
};

}
}
