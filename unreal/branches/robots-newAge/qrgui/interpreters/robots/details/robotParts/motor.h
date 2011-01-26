#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"

#include "robotPart.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Motor : public RobotPart
{
	Q_OBJECT

public:
	Motor(int const port, RobotCommunicationInterface *robotCommunicationInterface);
	void on(int speed);
	void on(int speed, long unsigned int degrees);
	void stop();
	void off();
	void resetMotorPosition(bool relative);

private:
	outputPort::OutputPortEnum mPort;

	void setOutputState(int speed, int mode
			, regulationMode::RegulationModeEnum regulation, int turnRatio, runState::RunStateEnum runState
			, unsigned long tachoLimit);
};

}
}
}
}
}
