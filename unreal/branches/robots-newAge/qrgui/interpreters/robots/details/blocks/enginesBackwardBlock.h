#pragma once

#include <QtCore/QObject>

#include "block.h"
#include "../robotParts/motor.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class EnginesBackwardBlock : public Block
{
	Q_OBJECT

public:
	EnginesBackwardBlock(robotParts::Motor &motor1, robotParts::Motor &motor2);
	virtual void run();

private:
	robotParts::Motor &mMotor1;
	robotParts::Motor &mMotor2;
};

}
}
}
}
}
