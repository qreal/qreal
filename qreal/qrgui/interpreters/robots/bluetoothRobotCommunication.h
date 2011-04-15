#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationInterface.h"
#include "details/bluetoothRobotCommunicationThread.h"
#include "details/robotCommandConstants.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class BluetoothRobotCommunication : public RobotCommunicationInterface
{
	Q_OBJECT

public:
	BluetoothRobotCommunication(QString const &portName);
	~BluetoothRobotCommunication();

	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, inputPort::InputPortEnum const &port);
	virtual void connect();
	virtual void disconnect();

	void setPortName(QString const &portName);

signals:
	void threadSend(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void threadSendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
					, details::lowLevelInputPort::InputPortEnum const &port);
	void threadConnect(QString portName);
	void threadReconnect(QString portName);
	void threadDisconnect();

private slots:
	void connectedSlot(bool success);
	void disconnectedSlot();
	void responseSlot(QObject* addressee, QByteArray const &buffer);

private:
	QString mPortName;
	QThread mRobotCommunicationThread;
	details::BluetoothRobotCommunicationThread mRobotCommunicationThreadObject;
};

}
}
}
