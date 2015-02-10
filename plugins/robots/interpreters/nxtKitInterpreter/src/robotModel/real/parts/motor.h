#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>
#include "src/robotModel/parts/nxtMotor.h"

#include "commandConstants.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Motor : public nxtKitInterpreter::robotModel::parts::NxtMotor
{
	Q_OBJECT

public:
	Motor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void on(int speed, bool breakMode = true) override;
	void on(int speed, long unsigned int degrees, bool breakMode = true) override;
	void stop(bool breakMode = true) override;
	void off() override;
	void resetMotorPosition(bool relative);

private:
	void setOutputState(int speed
			, int mode
			, enums::regulationMode::RegulationModeEnum regulation
			, int turnRatio
			, enums::runState::RunStateEnum runState
			, unsigned long tachoLimit);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
