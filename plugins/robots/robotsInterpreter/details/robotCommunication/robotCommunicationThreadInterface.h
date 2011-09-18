#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include "../robotsInterpreter/sensorConstants.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotCommunicationThreadInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~RobotCommunicationThreadInterface() {}
	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize) = 0;
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, inputPort::InputPortEnum const &port) = 0;
	virtual void connect(QString const &portName) = 0;
	virtual void disconnect() = 0;

signals:
	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);
};

}
}
}

