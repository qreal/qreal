#include "ev3EnginesForwardBlock.h"

#include "src/robotModel/parts/ev3Motor.h"

using namespace ev3KitInterpreter::blocks::details;
using namespace ev3KitInterpreter::robotModel::parts;

Ev3EnginesForwardBlock::Ev3EnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void Ev3EnginesForwardBlock::run()
{
	const int power = eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	for (Ev3Motor * const motor : parsePorts<Ev3Motor>()) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
