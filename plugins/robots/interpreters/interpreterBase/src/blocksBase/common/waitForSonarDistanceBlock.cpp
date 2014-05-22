#include "interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h"

#include "interpreterBase/robotModel/robotParts/rangeSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForSonarDistanceBlock::WaitForSonarDistanceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
		, interpreterBase::robotModel::DeviceInfo const &device)
	: WaitForSensorBlock(robotModel)
	, mDevice(device)
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
	return mDevice;
}
