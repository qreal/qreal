#pragma once

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>

class QextSerialPort;
class QTimer;

namespace nxt {
namespace communication {

class BluetoothRobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	BluetoothRobotCommunicationThread();
	~BluetoothRobotCommunicationThread();

public slots:
	void send(QObject *addressee, const QByteArray &buffer, const unsigned responseSize);
	void connect();
	void reconnect();
	void disconnect();

	void allowLongJobs(bool allow = true);
	void checkConsistency();

private slots:
	/// Checks if robot is connected
	void checkForConnection();

private:
	void send(const QByteArray &buffer, const unsigned responseSize
			, QByteArray &outputBuffer);
	void send(const QByteArray &buffer) const;
	QByteArray receive(int size) const;

	QextSerialPort *mPort;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
};

}
}
