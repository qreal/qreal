#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationInterface.h"
#include "details/bluetoothRobotCommunicationThread.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class BluetoothRobotCommunication : public RobotCommunicationInterface
{
	Q_OBJECT

public:
	BluetoothRobotCommunication(QString const &portName);

	virtual void send(QObject *addressee, QByteArray const &buffer);
	virtual void connect();
	virtual void disconnect();

	void setPortName(QString const &portName);

signals:
	void threadSend(QObject *addressee, QByteArray const &buffer);
	void threadConnect(QString portName);
	void threadReconnect(QString portName);
	void threadDisconnect();

private slots:
	void connectedSlot();
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
