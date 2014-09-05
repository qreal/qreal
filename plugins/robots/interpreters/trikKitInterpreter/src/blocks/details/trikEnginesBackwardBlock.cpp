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
	auto const result = -eval<int>("Power");
	if (!wereParserErrors()) {
		for (Motor * const motor : parsePorts<Motor>()) {
			motor->on(result);
		}

		emit done(mNextBlockId);
	}
}
