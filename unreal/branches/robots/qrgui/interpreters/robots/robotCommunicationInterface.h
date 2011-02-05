#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include "sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotCommunicationInterface : public QObject
{
	Q_OBJECT

public:
	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize) = 0;
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, inputPort::InputPortEnum const &port) = 0;
	virtual void connect() = 0;
	virtual void disconnect() = 0;

signals:
	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);
};

}
}
}
