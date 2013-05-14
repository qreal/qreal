#include "enginesBackwardBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EnginesBackwardBlock::EnginesBackwardBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3)
	: EngineCommandBlock(motor1, motor2, motor3)
{
}

void EnginesBackwardBlock::run()
{
	int const power = -evaluate("Power").toInt();
	bool const breakMode = stringProperty("Mode") == QString::fromUtf8("тормозить");
	QVector<bool> ports = parsePorts();
	for (int i = 0; i < 3; ++i) {
		if (ports[i]) {
			mMotors[i]->on(power, breakMode);
		}
	}

	emit done(mNextBlock);
}
