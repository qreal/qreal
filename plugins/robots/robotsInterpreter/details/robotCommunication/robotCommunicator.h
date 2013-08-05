#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationThreadInterface.h"
#include "../robotCommandConstants.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotCommunicator : public QObject
{
	Q_OBJECT

public:
	RobotCommunicator();
	~RobotCommunicator();

	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, robots::enums::inputPort::InputPortEnum const port);
	virtual void connect();
	virtual void disconnect();

	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

signals:
	void threadSend(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void threadSendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
			, robots::enums::inputPort::InputPortEnum const port);
	void threadConnect();
	void threadReconnect();
	void threadDisconnect();
	void errorOccured(QString const &message);

	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);

private slots:
	void connectedSlot(bool success);
	void disconnectedSlot();
	void responseSlot(QObject* addressee, QByteArray const &buffer);
	void onErrorOccured(QString const &message);

private:
	QThread mRobotCommunicationThread;
	RobotCommunicationThreadInterface *mRobotCommunicationThreadObject;
};

}
}
}
}
