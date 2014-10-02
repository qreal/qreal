#pragma once

#include "robotCommunicationThreadBase.h"

class QextSerialPort;
class QTimer;

namespace ev3KitInterpreter {
namespace communication {

class BluetoothRobotCommunicationThread : public RobotCommunicationThreadBase
{
	Q_OBJECT

public:
	BluetoothRobotCommunicationThread();
	~BluetoothRobotCommunicationThread();

public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect();
	void reconnect();
	void disconnect();

	void allowLongJobs(bool allow = true);
	void checkConsistency();

private slots:
	/// Checks if robot is connected
	void checkForConnection();

private:
	void send(QByteArray const &buffer, unsigned const responseSize
			, QByteArray &outputBuffer);
	void send(QByteArray const &buffer) const;
    void keepAlive();

	QByteArray receive(int size) const;

	QextSerialPort *mPort;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
};

}
}
