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
	for (int i = 0; i < 3; ++i) {
		if (ports[i]) {
			mMotors[i]->off();
		}
	}
	emit done(mNextBlock);
}
