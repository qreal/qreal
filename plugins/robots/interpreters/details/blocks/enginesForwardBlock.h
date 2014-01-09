#pragma once

#include <QtCore/QObject>

#include "engineCommandBlock.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class EnginesForwardBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	EnginesForwardBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3);
	virtual void run();
};

}
}
}
}
}
