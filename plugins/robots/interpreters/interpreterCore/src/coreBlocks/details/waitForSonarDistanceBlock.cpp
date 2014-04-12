#include "waitForSonarDistanceBlock.h"

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

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

interpreterBase::robotModel::DeviceInfo WaitForSonarDistanceBlock::device() const
{
	return DeviceInfo::create<robotParts::RangeSensor>();
}
