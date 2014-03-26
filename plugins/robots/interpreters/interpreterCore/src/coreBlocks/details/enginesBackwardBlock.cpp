#include "enginesBackwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace interpreterCore::coreBlocks::details;

EnginesBackwardBlock::EnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesBackwardBlock::run()
{
	int const power = -evaluate("Power").toInt();

	/// @todo Makes sense only for NXT.
	bool const breakMode = stringProperty("Mode") != QString::fromUtf8("скользить");

	for (interpreterBase::robotModel::robotParts::Motor * const motor : parsePorts()) {
		motor->on(power, breakMode);
	}

	emit done(mNextBlockId);
}
