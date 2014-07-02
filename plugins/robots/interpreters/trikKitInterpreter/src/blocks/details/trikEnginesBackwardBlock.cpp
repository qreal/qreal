#include "trikEnginesBackwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

TrikEnginesBackwardBlock::TrikEnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void TrikEnginesBackwardBlock::run()
{
	int const power = -evaluate("Power").toInt();

	for (Motor * const motor : parsePorts<Motor>()) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
