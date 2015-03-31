#pragma once

#include <trikKit/robotModel/parts/trikLed.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Led : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	Led(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);


	void setColor(const QString &color) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
