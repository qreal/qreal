#include "enginesBackwardBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EnginesBackwardBlock::EnginesBackwardBlock(robotParts::Motor &motor1, robotParts::Motor &motor2)
	: mMotor1(motor1)
	, mMotor2(motor2)
{
}

void EnginesBackwardBlock::run()
{
	mMotor1.on(-100);
	mMotor2.on(-100);
	emit done(mNextBlock);
}
