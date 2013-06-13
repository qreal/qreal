#include "engineCommandBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EngineCommandBlock::EngineCommandBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3)
{
	mMotors.append(&motor1);
	mMotors.append(&motor2);
	mMotors.append(&motor3);
}

void EngineCommandBlock::timeout()
{
	emit done(mNextBlock);
}
