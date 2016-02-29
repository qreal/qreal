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

class TCP_ROBOT_SIMULATOR_EXPORT TcpRobotSimulator : public QTcpServer
{
	Q_OBJECT

public:
	TcpRobotSimulator(int port);
	~TcpRobotSimulator() override;

	bool runProgramRequestReceived() const;
	bool configVersionRequestReceived() const;
	bool versionRequestReceived() const;

private:
	void incomingConnection(qintptr socketDescriptor) override;

	QScopedPointer<QThread> mConnectionThread;
	QScopedPointer<Connection> mConnection;
};

}
