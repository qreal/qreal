#include "trikEnginesForwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

TrikEnginesForwardBlock::TrikEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void TrikEnginesForwardBlock::run()
{
	auto const result = eval<int>("Power");
	if (!errorsOccured()) {
		for (Motor * const motor : parsePorts<Motor>()) {
			motor->on(result);
		}

		emit done(mNextBlockId);
	}
}
