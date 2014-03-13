#pragma once

#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public interpreterBase::robotModel::robotParts::Speaker
{
public:
	Speaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void beep(unsigned time);
	void playTone(unsigned freq, unsigned time);

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
