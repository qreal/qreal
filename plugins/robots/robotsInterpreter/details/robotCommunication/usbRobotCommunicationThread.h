#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationThreadInterface.h"
#include "../robotCommandConstants.h"
#include "fantom.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class UsbRobotCommunicationThread : public RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect(QString const &portName);
	void reconnect(QString const &portName);
	void disconnect();
	void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
			, inputPort::InputPortEnum const &port);

private:
	static const int kStatusNoError = 0;

	bool isOpen();

	void send(QByteArray const &buffer, unsigned const responseSize, QObject *addressee);

	bool mActive;
	unsigned long mNXTHandle;
	robotsInterpreter::robotCommunication::Fantom mFantom;
};

}
}
}
