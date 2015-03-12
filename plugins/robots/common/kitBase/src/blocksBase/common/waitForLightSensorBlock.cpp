#include "kitBase/blocksBase/common/waitForLightSensorBlock.h"

#include "kitBase/robotModel/robotParts/lightSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForLightSensorBlock::WaitForLightSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForLightSensorBlock::responseSlot(int reading)
{
	const int result = eval<int>("Percents");
	if (errorsOccured()) {
		processResponce(reading, result);
	}
}

kitBase::robotModel::DeviceInfo WaitForLightSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}
