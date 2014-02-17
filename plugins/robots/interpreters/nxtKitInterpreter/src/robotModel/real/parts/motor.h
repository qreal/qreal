#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Motor : public interpreterBase::robotModel::robotParts::Motor
{
	Q_OBJECT
public:
	Motor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator *robotCommunicator);

	void on(int speed, bool breakMode = true) override;
	void on(int speed, long unsigned int degrees, bool breakMode = true) override;
	void stop(bool breakMode = true) override;
	void off() override;
	void resetMotorPosition(bool relative);

protected:
	void doConfiguration();

private:
	void setOutputState(int speed
			, int mode
			, enums::regulationMode::RegulationModeEnum regulation
			, int turnRatio
			, enums::runState::RunStateEnum runState
			, unsigned long tachoLimit);

	utils::robotCommunication::RobotCommunicator *mRobotCommunicator;
};

}
}
}
}
