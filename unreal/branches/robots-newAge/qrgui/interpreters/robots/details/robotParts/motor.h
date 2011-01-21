#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"

#include "robotPart.h"

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

private:
	int mPort;

	void setOutputState(int speed, int mode, int regulation, int turnRatio, int runState, unsigned long tachoLimit);
};

}
}
}
}
}
