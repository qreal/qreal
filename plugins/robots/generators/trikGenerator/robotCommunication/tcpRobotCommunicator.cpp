#include "tcpRobotCommunicator.h"

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

using namespace trik;

static uint const port = 8888;

QString const errorMarker = "error: ";
QString const infoMarker = "info: ";

TcpRobotCommunicator::TcpRobotCommunicator(qReal::ErrorReporterInterface &errorReporter)
	: mErrorReporter(errorReporter)
{
	QObject::connect(&mSocket, SIGNAL(readyRead()), this, SLOT(onIncomeData()), Qt::DirectConnection);
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

	return true;
}

void TcpRobotCommunicator::onIncomeData()
{
	QStringList const messages = QString(mSocket.readAll()).split('\n');
	for (QString const &message : messages) {
		processIncommingMessage(message);
	}
}

void TcpRobotCommunicator::processIncommingMessage(QString const &message)
{
	if (message.startsWith(errorMarker)) {
		mErrorReporter.addError(message.mid(errorMarker.length()));
	} else if (message.startsWith(infoMarker)) {
		mErrorReporter.addInformation(message.mid(infoMarker.length()));
	} else {
		QLOG_INFO() << "Incomming message of unknown type: " << message;
	}
}

void TcpRobotCommunicator::connect()
{
	if (mSocket.state() == QTcpSocket::ConnectedState || mSocket.state() == QTcpSocket::ConnectingState) {
		return;
	}

	QString const server = qReal::SettingsManager::value("TrikTcpServer").toString();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		QLOG_ERROR() << "Unable to resolve host. Check server address and try again";
		return;
	}

	mSocket.connectToHost(hostAddress, static_cast<quint16>(port));
	bool const result = mSocket.waitForConnected(5000);
	if (!result) {
		QLOG_ERROR() << mSocket.errorString();
	}
}

void TcpRobotCommunicator::disconnect()
{
	if (mSocket.state() == QTcpSocket::ConnectedState) {
		mSocket.disconnectFromHost();
		mSocket.waitForDisconnected(3000);
	}
}
