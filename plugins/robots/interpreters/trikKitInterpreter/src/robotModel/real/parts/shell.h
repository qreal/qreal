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
	Shell(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void runCommand(const QString &command) override;

	void say(const QString &text) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
