#include "interpreterBase/blocksBase/common/waitForLightSensorBlock.h"

#include "interpreterBase/robotModel/robotParts/lightSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForLightSensorBlock::WaitForLightSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForLightSensorBlock::responseSlot(int reading)
{
	int const result = eval<int>("Percents");
	if (wereParserErrors()) {
		processResponce(reading, result);
	}
}

interpreterBase::robotModel::DeviceInfo WaitForLightSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}
