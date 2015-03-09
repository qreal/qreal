#pragma once

#include "robotModel/parts/nxtSpeaker.h"
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::NxtSpeaker
{
	Q_OBJECT

public:
	Speaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void playTone(unsigned freq, unsigned time) override;

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
