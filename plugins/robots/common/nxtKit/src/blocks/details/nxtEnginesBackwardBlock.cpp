#include "nxtEnginesBackwardBlock.h"

#include "nxtKit/robotModel/parts/nxtMotor.h"

using namespace nxt::blocks::details;
using namespace nxt::robotModel::parts;

NxtEnginesBackwardBlock::NxtEnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void NxtEnginesBackwardBlock::run()
{
	const int power = -eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	const bool breakMode = stringProperty("Mode") != "float";

	for (NxtMotor * const motor : parsePorts<NxtMotor>()) {
		motor->on(power, breakMode);
	}

	emit done(mNextBlockId);
}
