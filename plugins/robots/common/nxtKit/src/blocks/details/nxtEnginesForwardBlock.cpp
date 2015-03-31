#include "nxtEnginesForwardBlock.h"

#include "nxtKit/robotModel/parts/nxtMotor.h"

using namespace nxt::blocks::details;
using namespace nxt::robotModel::parts;

NxtEnginesForwardBlock::NxtEnginesForwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void NxtEnginesForwardBlock::run()
{
	const int power = eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	const bool breakMode = stringProperty("Mode") != "float";

	for (NxtMotor * const motor : parsePorts<NxtMotor>()) {
		motor->on(power, breakMode);
	}

	emit done(mNextBlockId);
}
