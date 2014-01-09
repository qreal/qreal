#include "waitForSonarDistanceBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForSonarDistanceBlock::WaitForSonarDistanceBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, robots::enums::sensorType::sonar)
{
}

void WaitForSonarDistanceBlock::responseSlot(int reading)
{
	int const targetDistance = evaluate("Distance").toInt();
	processResponce(reading, targetDistance);
}

interpreters::robots::details::robotParts::Sensor *WaitForSonarDistanceBlock::sensor() const
{
	return mRobotModel->sonarSensor(mPort);
}

QString WaitForSonarDistanceBlock::name() const
{
	return tr("Sonar sensor");
}
