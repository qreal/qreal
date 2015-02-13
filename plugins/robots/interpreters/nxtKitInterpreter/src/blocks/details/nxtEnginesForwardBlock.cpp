#include "nxtEnginesForwardBlock.h"

#include "src/robotModel/parts/nxtMotor.h"

using namespace nxtKitInterpreter::blocks::details;
using namespace nxtKitInterpreter::robotModel::parts;

NxtEnginesForwardBlock::NxtEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::EngineCommandBlock(robotModel)
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
