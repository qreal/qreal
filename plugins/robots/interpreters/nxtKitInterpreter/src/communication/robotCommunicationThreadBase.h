#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>
#include <interpreterBase/robotModel/portInfo.h>

namespace nxtKitInterpreter {
namespace communication {

class RobotCommunicationThreadBase : public utils::robotCommunication::RobotCommunicationThreadInterface
{
public:
	RobotCommunicationThreadBase();

	virtual void sendI2C(
			QObject *addressee, QByteArray const &buffer
			, unsigned const responseSize
			, interpreterBase::robotModel::PortInfo const &port
			);

protected:
	static int const i2cTimeout = 5000;

	class SleeperThread : public QThread
	{
	public:
		static void msleep(unsigned long msecs);
	};

	bool waitForI2CBytes(int bytes, interpreterBase::robotModel::PortInfo const &port);
	int i2cBytesReady(interpreterBase::robotModel::PortInfo const &port);

	virtual void send(QByteArray const &buffer, unsigned const responseSize, QByteArray &outputBuffer) = 0;

	char toNxtInputPort(interpreterBase::robotModel::PortInfo const &port);
};

}
}
