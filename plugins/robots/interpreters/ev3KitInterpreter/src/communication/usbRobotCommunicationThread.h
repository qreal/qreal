#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "robotCommunicationThreadBase.h"
//#include "fantom.h"

class QextSerialPort;

namespace nxtKitInterpreter {
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

	/// Checks that message requires response or not.
	/// @returns true, if there shall be a response.
	static bool isResponseNeeded(QByteArray const &buffer);

private:
	static const int kStatusNoError = 0;

	bool isOpen();
	static void debugPrint(QByteArray const &buffer, bool out);

	void send(QByteArray const &buffer, unsigned const responseSize
			, QByteArray &outputBuffer);

	bool mActive;
	unsigned long mNXTHandle;
    //Fantom mFantom;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;

	bool mStopped;
};

}
}
