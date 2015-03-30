#include "ev3EnginesBackwardBlock.h"

#include "ev3Kit/robotModel/parts/ev3Motor.h"

using namespace ev3::blocks::details;
using namespace ev3::robotModel::parts;

Ev3EnginesBackwardBlock::Ev3EnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void Ev3EnginesBackwardBlock::run()
{
	const int power = -eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	for (Ev3Motor * const motor : parsePorts<Ev3Motor>()) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
