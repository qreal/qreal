#pragma once

#include "robotModel/parts/trikMotionSensor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class MotionSensor : public robotModel::parts::TrikMotionSensor
{
	Q_OBJECT

public:
	MotionSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
