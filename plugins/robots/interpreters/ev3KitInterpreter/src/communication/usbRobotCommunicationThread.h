#pragma once

#include "robotCommunicationThreadBase.h"
#include <libusb.h>

class QTimer;

namespace ev3KitInterpreter {
namespace communication {

class UsbRobotCommunicationThread : public RobotCommunicationThreadBase
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

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

	QByteArray receive(int size) const;

	libusb_device_handle *mHandle;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;
};

}
}
