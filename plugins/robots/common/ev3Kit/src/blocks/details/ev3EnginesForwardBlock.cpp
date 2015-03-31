#include "ev3EnginesForwardBlock.h"

#include "ev3Kit/robotModel/parts/ev3Motor.h"

using namespace ev3::blocks::details;
using namespace ev3::robotModel::parts;

Ev3EnginesForwardBlock::Ev3EnginesForwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::EngineCommandBlock(robotModel)
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
