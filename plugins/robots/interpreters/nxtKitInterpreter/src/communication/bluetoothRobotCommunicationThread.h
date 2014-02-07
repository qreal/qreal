#pragma once

#include "robotCommunicationThreadBase.h"

class QextSerialPort;

namespace nxtKitInterpreter {
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

	void sendI2C(
			QObject *addressee
			, QByteArray const &buffer
			, unsigned const responseSize
			, interpreterBase::robotModel::PortInfo const &port
			);

	void allowLongJobs(bool allow = true);
	void checkConsistency();

private:
	void send(QByteArray const &buffer, unsigned const responseSize
			, QByteArray &outputBuffer);
	void send(QByteArray const &buffer) const;
	QByteArray receive(int size) const;

	QextSerialPort *mPort;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;

private slots:
	/// Checks if robot is connected
	void checkForConnection();
};

}
}
