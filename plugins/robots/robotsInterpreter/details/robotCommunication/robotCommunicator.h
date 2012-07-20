#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationThreadInterface.h"
#include "../robotCommandConstants.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class RobotCommunicator : public QObject
{
	Q_OBJECT

public:
	RobotCommunicator(QString const &portName);
	~RobotCommunicator();

	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, inputPort::InputPortEnum const &port);
	virtual void connect();
	virtual void disconnect();

	void setPortName(QString const &portName);
	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

signals:
	void threadSend(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void threadSendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
					, inputPort::InputPortEnum const &port);
	void threadConnect(QString portName);
	void threadReconnect(QString portName);
	void threadDisconnect();

	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);

private slots:
	void connectedSlot(bool success);
	void disconnectedSlot();
	void responseSlot(QObject* addressee, QByteArray const &buffer);

private:
	QString mPortName;
	QThread mRobotCommunicationThread;
	RobotCommunicationThreadInterface *mRobotCommunicationThreadObject;
};

}
}
}
