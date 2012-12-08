#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "block.h"
#include "../robotParts/motor.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class EngineCommandBlock : public Block
{
	Q_OBJECT

public:
	EngineCommandBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3);
	virtual void run() = 0;

protected:
	QVector<robotParts::Motor *> mMotors;  // Doesn't have ownership.

	QVector<bool> parsePorts();
};

}
}
}
}
}
