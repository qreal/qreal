#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "utils/utilsDeclSpec.h"
#include "robotCommunicationThreadInterface.h"

class QextSerialPort;

namespace utils {
namespace robotCommunication {

class ROBOTS_UTILS_EXPORT RobotCommunicator : public QObject
{
	Q_OBJECT

public:
	RobotCommunicator();
	~RobotCommunicator();

	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
//	virtual void sendI2C(QObject *addressee, QByteArray const &buffer
//			, unsigned const responseSize, robots::enums::inputPort::InputPortEnum const port);
	virtual void connect();
	virtual void disconnect();

	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

signals:
	void threadSend(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
//	void threadSendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
//			, robots::enums::inputPort::InputPortEnum const port);
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
