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
	QObject::connect(&mSocket, SIGNAL(readyRead()), this, SLOT(onIncomingData()), Qt::DirectConnection);
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

	send("file:" + fileNameOnARobot + ":" + fileContents);

	return true;
}

bool TcpRobotCommunicator::runProgram(QString const &programName)
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("run:" + programName);

	return true;
}

bool TcpRobotCommunicator::runDirectCommand(QString const &directCommand)
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("direct:" + directCommand);

	return true;
}

bool TcpRobotCommunicator::stopRobot()
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("stop");

	return true;
}

void TcpRobotCommunicator::onIncomingData()
{
	if (!mSocket.isValid()) {
		return;
	}

	QByteArray const &data = mSocket.readAll();
	mBuffer.append(data);

	while (!mBuffer.isEmpty()) {
		if (mExpectedBytes == 0) {
			// Determining the length of a message.
			int const delimiterIndex = mBuffer.indexOf(':');
			if (delimiterIndex == -1) {
				// We did not receive full message length yet.
				return;
			} else {
				QByteArray const length = mBuffer.left(delimiterIndex);
				mBuffer = mBuffer.mid(delimiterIndex + 1);
				bool ok;
				mExpectedBytes = length.toInt(&ok);
				if (!ok) {
					QLOG_ERROR() << "Malformed message, can not determine message length from this:" << length;
					mExpectedBytes = 0;
				}
			}
		} else {
			if (mBuffer.size() >= mExpectedBytes) {
				QByteArray const message = mBuffer.left(mExpectedBytes);
				mBuffer = mBuffer.mid(mExpectedBytes);

				processIncomingMessage(message);

				mExpectedBytes = 0;
			} else {
				// We don't have all message yet.
				return;
			}
		}
	}
}

void TcpRobotCommunicator::processIncomingMessage(QString const &message)
{
	if (message.startsWith(errorMarker)) {
		mErrorReporter.addError(message.mid(errorMarker.length()));
	} else if (message.startsWith(infoMarker)) {
		mErrorReporter.addInformation(message.mid(infoMarker.length()));
	} else {
		QLOG_INFO() << "Incoming message of unknown type: " << message;
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
		QLOG_ERROR() << "Unable to resolve host.";
		return;
	}

	mSocket.connectToHost(hostAddress, static_cast<quint16>(port));
	bool const result = mSocket.waitForConnected(5000);
	if (!result) {
		QLOG_ERROR() << mSocket.errorString();
	}

	mBuffer.clear();
	mExpectedBytes = 0;
}

void TcpRobotCommunicator::disconnect()
{
	if (mSocket.state() == QTcpSocket::ConnectedState) {
		mSocket.disconnectFromHost();
		mSocket.waitForDisconnected(3000);
	}
}

void TcpRobotCommunicator::send(QString const &data)
{
	QByteArray dataByteArray = data.toUtf8();
	dataByteArray = QByteArray::number(dataByteArray.size()) + ':' + dataByteArray;
	mSocket.write(dataByteArray);
	if (!mSocket.waitForBytesWritten(3000)) {
		QLOG_ERROR() << "Unable to send data" << data << "to" << mSocket.peerAddress();
	}
}
