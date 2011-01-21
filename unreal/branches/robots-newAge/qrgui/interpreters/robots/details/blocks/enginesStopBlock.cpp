#include "enginesStopBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EnginesStopBlock::EnginesStopBlock(robotParts::Motor &motor1, robotParts::Motor &motor2)
	: mMotor1(motor1)
	, mMotor2(motor2)
{
}

void EnginesStopBlock::run()
{
	mMotor1.off();
	mMotor2.off();
	emit done(mNextBlock);
}
