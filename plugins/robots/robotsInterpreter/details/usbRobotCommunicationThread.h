#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "robotCommunicationThreadInterface.h"
#include "robotCommandConstants.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {

class UsbRobotCommunicationThread : public RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	explicit UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread() {}

	QString getBluetoothName();
	void setBluetoothName(QString name);
	bool getUseBT();
	void setUseBT(bool value);
	unsigned long getBST();
	void setBST(unsigned long value);
	QString getPort();
	void setPort(QString value);
	QString playTone(unsigned freq, unsigned time);

	QList<QString> NXTListWidget();
	void runMotor(unsigned motorName, int speed);
	void stopMotor(unsigned motorName);
	bool isOpen();


public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect(QString const &portName);
	void reconnect(QString const &portName);
	void disconnect();
	void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
			, inputPort::InputPortEnum const &port);

private:
	bool getIsOpen();
	void send(QByteArray const &buffer, unsigned const responseSize, QObject *addressee);
	QByteArray receive(int size) const;

	bool mActive;
	QString mResPort;
	QString mBTName;
	bool mUseBT;
	unsigned long mBluetoothSearchTimeout;
	QString mPort;
	unsigned long mNXTHandle;
	unsigned mAPower;
	unsigned mBPower;
};

}
}
}
