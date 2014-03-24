#include <QtNetwork/QHostAddress>

#include "tcpRobotCommunicationThread.h"
#include "../../../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots::details;

TcpRobotCommunicationThread::TcpRobotCommunicationThread()
	: mSocket(nullptr)
{
}

TcpRobotCommunicationThread::~TcpRobotCommunicationThread()
{
}

void TcpRobotCommunicationThread::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	if (!mSocket) {
		return;
	}

	mSocket->write(buffer);
	mSocket->waitForBytesWritten();
	if (responseSize == 2) {
		mSocket->waitForReadyRead();
		emit response(addressee, mSocket->read(responseSize));
	} else {
		emit response(addressee, QByteArray());
	}
}

void TcpRobotCommunicationThread::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, robots::enums::inputPort::InputPortEnum port)
{
	Q_UNUSED(addressee)
	Q_UNUSED(buffer)
	Q_UNUSED(responseSize)
	Q_UNUSED(port)
}

void TcpRobotCommunicationThread::connect()
{
	QString const server = SettingsManager::value("tcpServer").toString();
	uint const port = SettingsManager::value("tcpPort").toUInt();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		QString const message = tr("Unable to resolve %1. Check server address and try again.");
		emit errorOccured(message.arg(server));
		emit connected(false);
		return;
	}
	mSocket = new QTcpSocket(this);
	mSocket->connectToHost(hostAddress, static_cast<quint16>(port));
	emit connected(mSocket->waitForConnected());
}

void TcpRobotCommunicationThread::disconnect()
{
	if (!mSocket) {
		return;
	}

	mSocket->disconnectFromHost();
	mSocket->waitForDisconnected();
	emit disconnected();
}

void TcpRobotCommunicationThread::reconnect()
{
	connect();
}

void TcpRobotCommunicationThread::allowLongJobs(bool allow)
{
	Q_UNUSED(allow)
}

void TcpRobotCommunicationThread::checkConsistency()
{
}
