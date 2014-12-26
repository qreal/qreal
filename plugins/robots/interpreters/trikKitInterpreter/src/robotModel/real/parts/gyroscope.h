#pragma once

#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Gyroscope : public interpreterBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	Gyroscope(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;

public slots:
	void onIncomingData(QString const &portName, int value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
