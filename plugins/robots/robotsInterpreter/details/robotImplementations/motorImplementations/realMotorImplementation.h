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
	virtual void on(int speed);
	virtual void on(int speed, long unsigned int degrees);
	virtual void stop();
	virtual void off();
	virtual void resetMotorPosition(bool relative);
private:
	void setOutputState(int speed, int mode
			, regulationMode::RegulationModeEnum regulation, int turnRatio, runState::RunStateEnum runState
			, unsigned long tachoLimit);
	RobotCommunicator *mRobotCommunicationInterface;
};

}
}
}
}
}
}
