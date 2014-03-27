#include "waitForSonarDistanceBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForSonarDistanceBlock::WaitForSonarDistanceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForSonarDistanceBlock::~WaitForSonarDistanceBlock()
{
}

void WaitForSonarDistanceBlock::responseSlot(int reading)
{
	int const targetDistance = evaluate("Distance").toInt();
	processResponce(reading, targetDistance);
}

//interpreters::robots::details::robotParts::Sensor *WaitForSonarDistanceBlock::sensor() const
//{
//	return mRobotModel->sonarSensor(mPort);
//}

QString WaitForSonarDistanceBlock::name() const
{
	return tr("Sonar sensor");
}
