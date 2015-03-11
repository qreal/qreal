#include "kitBase/blocksBase/common/waitForSonarDistanceBlock.h"

#include "kitBase/robotModel/robotParts/rangeSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForSonarDistanceBlock::WaitForSonarDistanceBlock(kitBase::robotModel::RobotModelInterface &robotModel
		, const kitBase::robotModel::DeviceInfo &device)
	: WaitForSensorBlock(robotModel)
	, mDevice(device)
{
}

void WaitForSonarDistanceBlock::responseSlot(int reading)
{
	const int targetDistance = eval<int>("Distance");
	if (!errorsOccured()) {
		processResponce(reading, targetDistance);
	}
}

kitBase::robotModel::DeviceInfo WaitForSonarDistanceBlock::device() const
{
	return mDevice;
}
