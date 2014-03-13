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

	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void send(QByteArray const &buffer, unsigned const responseSize, QByteArray &outputBuffer);
	void connect();
	void disconnect();

	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

signals:
	void errorOccured(QString const &message);
	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);

private:
	QThread mRobotCommunicationThread;
	RobotCommunicationThreadInterface *mRobotCommunicationThreadObject;
};

}
}
