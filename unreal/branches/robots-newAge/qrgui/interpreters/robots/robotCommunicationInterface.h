#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

namespace qReal {
namespace interpreters {
namespace robots {

class RobotCommunicationInterface : public QObject
{
	Q_OBJECT

public:
	virtual void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize) = 0;
	virtual void connect() = 0;
	virtual void disconnect() = 0;

signals:
	void connected();
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);
};

}
}
}
