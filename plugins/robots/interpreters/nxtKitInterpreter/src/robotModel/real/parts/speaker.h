#pragma once

#include <nxtKit/robotModel/parts/nxtSpeaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::NxtSpeaker
{
	Q_OBJECT

public:
	Speaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void playTone(unsigned freq, unsigned time) override;

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
