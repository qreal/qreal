#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "robotCommunicationThreadInterface.h"
#include "../robotCommandConstants.h"

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class BluetoothRobotCommunicationThread : public RobotCommunicationThreadInterface {
	Q_OBJECT

public:
	BluetoothRobotCommunicationThread();
	~BluetoothRobotCommunicationThread();

public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect(QString const &portName);
	void reconnect(QString const &portName);
	void disconnect();
	void sendI2C(QObject *addressee, QByteArray const &buffer, unsigned const responseSize
			, inputPort::InputPortEnum const &port);
	void allowLongJobs(bool allow = true);
	void checkConsistency();

private:
	class SleeperThread : public QThread
	{
	public:
		static void msleep(unsigned long msecs)
		{
			QThread::msleep(msecs);
		}
	};

	bool waitForBytes(int bytes, inputPort::InputPortEnum const &port) const;
	int i2cBytesReady(inputPort::InputPortEnum const &port) const;

	void send(QByteArray const &buffer) const;
	QByteArray receive(int size) const;

	static int const timeout = 5000;

	QextSerialPort *mPort;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;

private slots:
	/// Checks if robot is connected
	void checkForConnection();
};

}
}
}
}
