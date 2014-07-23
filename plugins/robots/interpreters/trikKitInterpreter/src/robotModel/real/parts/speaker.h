#pragma once

#include "robotModel/parts/trikSpeaker.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::TrikSpeaker
{
	Q_OBJECT

public:
	Speaker(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void play(QString const &filePath) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
