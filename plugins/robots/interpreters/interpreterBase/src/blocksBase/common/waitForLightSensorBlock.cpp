#include "interpreterBase/blocksBase/common/waitForLightSensorBlock.h"

#include "interpreterBase/robotModel/robotParts/lightSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForLightSensorBlock::WaitForLightSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForLightSensorBlock::~WaitForLightSensorBlock()
{
}

void WaitForLightSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Percents").toInt();
	processResponce(reading, targetPercents);
}

interpreterBase::robotModel::DeviceInfo WaitForLightSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}
