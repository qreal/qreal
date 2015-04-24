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

#include "tcpRobotCommunicator.h"

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrutils/inFile.h>

using namespace trik;

static uint const port = 8888;

TcpRobotCommunicator::TcpRobotCommunicator()
{
}

TcpRobotCommunicator::~TcpRobotCommunicator()
{
	disconnect();
}

bool TcpRobotCommunicator::uploadProgram(QString const &programName)
{
	if (programName.isEmpty()) {
		return false;
	}

	QString fileContents;
	try {
		fileContents = utils::InFile::readAll(programName);
	} catch (qReal::Exception const &) {
		return false;
	}

	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	QString const &fileNameOnARobot = QFileInfo(programName).fileName();

	QString const command = "file:" + fileNameOnARobot + ":" + fileContents;
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten(3000);

	disconnect();

	return true;
}

bool TcpRobotCommunicator::runProgram(QString const &programName)
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	QString const command = "run:" + programName;
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten(3000);

	disconnect();

	return true;
}

bool TcpRobotCommunicator::runDirectCommand(QString const &directCommand)
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	QString const command = "direct:" + directCommand;
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten(3000);

	disconnect();

	return true;
}

bool TcpRobotCommunicator::stopRobot()
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	QString const command = "stop";
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten(3000);

	disconnect();

	return true;
}

void TcpRobotCommunicator::connect()
{
	QString const server = qReal::SettingsManager::value("TrikTcpServer").toString();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		qDebug() << "Unable to resolve host. Check server address and try again";
		return;
	}

	mSocket.connectToHost(hostAddress, static_cast<quint16>(port));
	bool const result = mSocket.waitForConnected(5000);
	if (!result) {
		qDebug() << mSocket.errorString();
	}
}

void TcpRobotCommunicator::disconnect()
{
	if (mSocket.state() == QTcpSocket::ConnectedState) {
		mSocket.disconnectFromHost();
		mSocket.waitForDisconnected(3000);
	}
}
