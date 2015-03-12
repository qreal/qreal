#pragma once

#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Gyroscope : public kitBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	Gyroscope(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;

public slots:
	void onIncomingData(const QString &portName, int value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
