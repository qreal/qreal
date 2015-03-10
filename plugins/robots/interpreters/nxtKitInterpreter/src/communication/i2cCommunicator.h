#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QThread>

#include <kitBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace communication {

class I2CCommunicator
{
public:
	explicit I2CCommunicator(utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void sendI2C(QObject *addressee, const QByteArray &buffer
			, const int responseSize
			, const kitBase::robotModel::PortInfo &port);

private:
	static const int i2cTimeout = 5000;

	bool waitForI2CBytes(int bytes, const kitBase::robotModel::PortInfo &port);
	int i2cBytesReady(const kitBase::robotModel::PortInfo &port);

	char toNxtInputPort(const kitBase::robotModel::PortInfo &port);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
