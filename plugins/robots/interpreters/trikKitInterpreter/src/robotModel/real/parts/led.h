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
	Led(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);


	void setColor(const QString &color) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
