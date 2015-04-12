#pragma once

#include <trikKit/robotModel/parts/trikShell.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Shell : public robotModel::parts::TrikShell
{
	Q_OBJECT

public:
	Shell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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
