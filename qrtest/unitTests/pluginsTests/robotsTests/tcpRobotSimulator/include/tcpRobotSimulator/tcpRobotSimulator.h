/* Copyright 2016 CyberTech Labs Ltd.
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

#include <QtNetwork/QTcpServer>
#include <QtCore/QScopedPointer>

#include "declSpec.h"

class QThread;

namespace tcpRobotSimulator {

class Connection;

/// TCP server that simulates TRIK robot behavior (at least, as required by run program protocol).
class TCP_ROBOT_SIMULATOR_EXPORT TcpRobotSimulator : public QTcpServer
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param port - port to listen to.
	explicit TcpRobotSimulator(int port);

	~TcpRobotSimulator() override;

	/// Sets robot config version which server shall use to reply to "configVersion" command.
	void setConfigVersion(const QString &configVersion);

	/// Check that server received "run" command.
	bool runProgramRequestReceived() const;

	/// Check that server received "configVersion" command.
	bool configVersionRequestReceived() const;

	/// Check that server received "version" command.
	bool versionRequestReceived() const;

signals:
	/// Emitted when "run" command received.
	void runProgramRequestReceivedSignal();

private:
	/// Called when somebody tries to connect to server. Note that in a given time there can only be one connection
	/// opened.
	void incomingConnection(qintptr socketDescriptor) override;

	/// Separate thread for a connection object.
	QScopedPointer<QThread> mConnectionThread;

	/// Connection object, handles actual interaction with client.
	QScopedPointer<Connection> mConnection;

	/// Robot casing version used to respond to "configVersion" command.
	QString mConfigVersion;
};

}
