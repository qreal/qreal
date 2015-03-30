#pragma once

#include <trikKit/robotModel/parts/trikSpeaker.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::TrikSpeaker
{
	Q_OBJECT

public:
	Speaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void play(const QString &filePath) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
