#include "interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h"

#include "interpreterBase/robotModel/robotParts/rangeSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForSonarDistanceBlock::WaitForSonarDistanceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
		, const interpreterBase::robotModel::DeviceInfo &device)
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

interpreterBase::robotModel::DeviceInfo WaitForSonarDistanceBlock::device() const
{
	return mDevice;
}
