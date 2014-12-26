#pragma once

#include "robotModel/parts/trikShell.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Shell : public robotModel::parts::TrikShell
{
	Q_OBJECT

public:
	Shell(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void runCommand(QString const &command) override;

	void say(QString const &text) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
