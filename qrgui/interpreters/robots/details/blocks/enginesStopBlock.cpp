#include "enginesStopBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EnginesStopBlock::EnginesStopBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3)
	: EngineCommandBlock(motor1, motor2, motor3)
{
}

void EnginesStopBlock::run()
{
	QVector<bool> ports = parsePorts();
	if (ports[0])
		mMotor1.off();
	if (ports[1])
		mMotor2.off();
	if (ports[2])
		mMotor3.off();
	emit done(mNextBlock);
}
