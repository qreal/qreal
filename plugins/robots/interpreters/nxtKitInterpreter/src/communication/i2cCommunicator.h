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

	void sendI2C(QObject *addressee, QByteArray const &buffer
			, int const responseSize
			, interpreterBase::robotModel::PortInfo const &port);

private:
	static int const i2cTimeout = 5000;

	bool waitForI2CBytes(int bytes, interpreterBase::robotModel::PortInfo const &port);
	int i2cBytesReady(interpreterBase::robotModel::PortInfo const &port);

	char toNxtInputPort(interpreterBase::robotModel::PortInfo const &port);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
