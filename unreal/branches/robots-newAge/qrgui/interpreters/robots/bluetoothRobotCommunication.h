#pragma once

#include <QtCore/QString>

#include "robotCommunicationInterface.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class BluetoothRobotCommunication : public RobotCommunicationInterface
{
public:
	BluetoothRobotCommunication(QString const &portName);

	virtual void send(QByteArray const &buffer);
	virtual void connect();
	virtual void disconnect();

	void setPortName(QString const &portName);
private:
	QString mPortName;
	QextSerialPort *mPort;
};

}
}
}
