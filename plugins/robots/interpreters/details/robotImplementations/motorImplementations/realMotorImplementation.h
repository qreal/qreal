#pragma once
#include "abstractMotorImplementation.h"
#include "../../robotCommunication/robotCommunicator.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace motorImplementations {

class RealMotorImplementation : public AbstractMotorImplementation
{
	Q_OBJECT
public:
	RealMotorImplementation(int const port, RobotCommunicator *robotCommunicationInterface);
	virtual void on(int speed, bool breakMode = true);
	virtual void on(int speed, long unsigned int degrees, bool breakMode = true);
	virtual void stop(bool breakMode = true);
	virtual void off();
	virtual void resetMotorPosition(bool relative);
private:
	void setOutputState(int speed, int mode
			, enums::regulationMode::RegulationModeEnum regulation, int turnRatio, enums::runState::RunStateEnum runState
			, unsigned long tachoLimit);
	RobotCommunicator *mRobotCommunicationInterface;
};

}
}
}
}
}
}
