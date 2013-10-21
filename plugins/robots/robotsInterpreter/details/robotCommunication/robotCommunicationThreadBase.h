#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "robotCommunicationThreadInterface.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotCommunicationThreadBase : public RobotCommunicationThreadInterface
{
public:
	RobotCommunicationThreadBase();

	virtual void sendI2C(
			QObject *addressee, QByteArray const &buffer
			, unsigned const responseSize
			, robots::enums::inputPort::InputPortEnum const port
			);

protected:
	static int const i2cTimeout = 5000;

	class SleeperThread : public QThread
	{
	public:
		static void msleep(unsigned long msecs)
		{
			QThread::msleep(msecs);
		}
	};

	bool waitForI2CBytes(int bytes, robots::enums::inputPort::InputPortEnum port);
	int i2cBytesReady(robots::enums::inputPort::InputPortEnum port);

	virtual void send(QByteArray const &buffer, unsigned const responseSize, QByteArray &outputBuffer) = 0;
};

}
}
}
}
