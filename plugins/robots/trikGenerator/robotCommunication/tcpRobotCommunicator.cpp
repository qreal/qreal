#include "tcpRobotCommunicator.h"

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "../../../../qrkernel/settingsManager.h"
#include "../../../../qrutils/inFile.h"

using namespace qReal::robots::generators::trik;

TcpRobotCommunicator::TcpRobotCommunicator()
{
}

TcpRobotCommunicator::~TcpRobotCommunicator()
{
	disconnect();
}

bool TcpRobotCommunicator::uploadProgram(QString const &programName)
{
	QString const fileContents = utils::InFile::readAll(programName);
	connect();
	if (!mSocket.isValid()) {
		return false;
	}

	QString const &fileNameOnARobot = QFileInfo(programName).fileName();

	QString const command = "file:" + fileNameOnARobot + ":" + fileContents;
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten();

	return true;
}

bool TcpRobotCommunicator::runProgram(QString const &programName)
{
	connect();
	if (!mSocket.isValid()) {
		return false;
	}

	QString const command = "run:" + programName;
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten();

	return true;
}

bool TcpRobotCommunicator::stopRobot()
{
	connect();
	if (!mSocket.isValid()) {
		return false;
	}

	QString const command = "stop";
	mSocket.write(command.toUtf8());
	mSocket.waitForBytesWritten();

	return true;
}

void TcpRobotCommunicator::connect()
{
	QString const server = qReal::SettingsManager::value("tcpServer").toString();
	uint const port = qReal::SettingsManager::value("tcpPort").toUInt();
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
	mSocket.disconnectFromHost();
	mSocket.waitForDisconnected();
}
