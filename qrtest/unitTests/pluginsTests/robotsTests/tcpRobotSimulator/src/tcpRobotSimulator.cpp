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

#include "tcpRobotSimulator/tcpRobotSimulator.h"

#include <QtCore/QThread>

#include <QtCore/QDebug>

#include "connection.h"

using namespace tcpRobotSimulator;

static const int port = 8888;

TcpRobotSimulator::TcpRobotSimulator()
{
	listen(QHostAddress::LocalHost, port);
}

TcpRobotSimulator::~TcpRobotSimulator()
{
}

void TcpRobotSimulator::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "TcpRobotSimulator::incomingConnection";
	qDebug() << "Thread:" << thread();

	mConnection.reset(new Connection(Protocol::messageLength, Heartbeat::use));
	mConnectionThread.reset(new QThread());
	connect(mConnectionThread.data(), &QThread::finished, mConnectionThread.data(), &QThread::deleteLater);
	mConnection->moveToThread(mConnectionThread.data());
	mConnectionThread->start();
	QMetaObject::invokeMethod(mConnection.data(), "init", Q_ARG(int, socketDescriptor));
}

bool TcpRobotSimulator::runProgramRequestReceived() const
{
	return mConnection->runProgramRequestReceived();
}

bool TcpRobotSimulator::configVersionRequestReceived() const
{
	return mConnection->configVersionRequestReceived();
}

bool TcpRobotSimulator::versionRequestReceived() const
{
	return mConnection->versionRequestReceived();
}
