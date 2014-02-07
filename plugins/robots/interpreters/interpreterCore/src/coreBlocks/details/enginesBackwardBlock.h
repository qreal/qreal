#pragma once

#include <QtCore/QObject>

#include "engineCommandBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class EnginesBackwardBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	EnginesBackwardBlock(/*robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3*/);
	void run() override;
};

}
}
}
