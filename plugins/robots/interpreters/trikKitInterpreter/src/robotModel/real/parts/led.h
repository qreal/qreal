#pragma once

#include "robotModel/parts/trikLed.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Led : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	Led(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);


	void setColor(QString const &color) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
