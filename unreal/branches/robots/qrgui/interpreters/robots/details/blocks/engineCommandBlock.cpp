#include "engineCommandBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EngineCommandBlock::EngineCommandBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3)
	: mMotor1(motor1)
	, mMotor2(motor2)
	, mMotor3(motor3)
{
}

QVector<bool> EngineCommandBlock::parsePorts()
{
	QString const ports = stringProperty("Ports");
	QVector<bool> result(3, false);
	QStringList splitted = ports.split(',', QString::SkipEmptyParts);
	foreach (QString port, splitted) {
		if (port.trimmed() == "A")
			result[0] = true;
		else if (port.trimmed() == "B")
			result[1] = true;
		else if (port.trimmed() == "C")
			result[2] = true;
	}
	return result;
}
