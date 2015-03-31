#include "trikEnginesBackwardBlock.h"

#include <kitBase/robotModel/robotParts/motor.h>

using namespace trik::blocks::details;
using namespace kitBase::robotModel::robotParts;

TrikEnginesBackwardBlock::TrikEnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void TrikEnginesBackwardBlock::run()
{
	const auto result = -eval<int>("Power");
	if (!errorsOccured()) {
		for (Motor * const motor : parsePorts<Motor>()) {
			motor->on(result);
		}

		emit done(mNextBlockId);
	}
}
