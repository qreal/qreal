#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QThread>

#include <interpreterBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace communication {

class I2CCommunicator
{
public:
	explicit I2CCommunicator(utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void sendI2C(QObject *addressee, const QByteArray &buffer
			, const int responseSize
			, const interpreterBase::robotModel::PortInfo &port);

private:
	static const int i2cTimeout = 5000;

	bool waitForI2CBytes(int bytes, const interpreterBase::robotModel::PortInfo &port);
	int i2cBytesReady(const interpreterBase::robotModel::PortInfo &port);

	char toNxtInputPort(const interpreterBase::robotModel::PortInfo &port);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
