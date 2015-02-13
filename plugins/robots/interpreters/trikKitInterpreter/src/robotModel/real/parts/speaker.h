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
	Speaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void play(const QString &filePath) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
