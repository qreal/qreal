#include "enginesForwardBlock.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace interpreters::robots::details::blocks;

EnginesForwardBlock::EnginesForwardBlock(robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3)
	: EngineCommandBlock(motor1, motor2, motor3)
{
}

void EnginesForwardBlock::run()
{
	qDebug() <<"EnginesForwardBlock run";
	int const power = evaluate("Power").toInt();
	int const tachoLimit = evaluate("TachoLimit").toInt();
	QVector<bool> ports = parsePorts();
	if (ports[0])
		mMotor1.on(power, tachoLimit);
	if (ports[1])
		mMotor2.on(power, tachoLimit);
	if (ports[2])
		mMotor3.on(power, tachoLimit);

	emit done(mNextBlock);
}
