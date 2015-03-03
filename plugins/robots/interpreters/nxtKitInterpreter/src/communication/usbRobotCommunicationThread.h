#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>
#include "fantom.h"

class QextSerialPort;

namespace nxtKitInterpreter {
namespace communication {

class UsbRobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	UsbRobotCommunicationThread();
	~UsbRobotCommunicationThread();

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

	/// Checks that message requires response or not.
	/// @returns true, if there shall be a response.
	static bool isResponseNeeded(const QByteArray &buffer);

private:
	static const int kStatusNoError = 0;

	bool isOpen();
	static void debugPrint(const QByteArray &buffer, bool out);

	void send(const QByteArray &buffer, const unsigned responseSize
			, QByteArray &outputBuffer);

	bool mActive;
	unsigned long mNXTHandle;
	Fantom mFantom;

	/// Timer that sends messages to robot to check that connection is still alive
	QTimer *mKeepAliveTimer;

	bool mStopped;
};

}
}
