#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(interpreterBase::robotModel::DeviceInfo const &info
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
