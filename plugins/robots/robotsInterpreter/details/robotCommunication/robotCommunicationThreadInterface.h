#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include "../robotsInterpreter/sensorConstants.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotCommunicationThreadInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~RobotCommunicationThreadInterface() {}

public slots:
	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize) = 0;
	virtual void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize, robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void connect() = 0;
	virtual void disconnect() = 0;
	virtual void reconnect() = 0;
	virtual void allowLongJobs(bool allow = true) = 0;
	virtual void checkConsistency() = 0;

signals:
	void connected(bool success);
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);
	void errorOccured(QString const &message);
};

}
}
}
}
