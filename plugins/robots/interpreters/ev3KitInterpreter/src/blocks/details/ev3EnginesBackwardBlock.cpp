#include "ev3EnginesBackwardBlock.h"

#include "src/robotModel/parts/ev3Motor.h"

using namespace ev3KitInterpreter::blocks::details;
using namespace ev3KitInterpreter::robotModel::parts;

Ev3EnginesBackwardBlock::Ev3EnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void Ev3EnginesBackwardBlock::run()
{
	int const power = -eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	for (Ev3Motor * const motor : parsePorts<Ev3Motor>()) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
