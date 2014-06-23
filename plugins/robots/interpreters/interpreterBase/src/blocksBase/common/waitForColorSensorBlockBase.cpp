#include "interpreterBase/blocksBase/common/waitForColorSensorBlockBase.h"

#include "interpreterBase/robotModel/robotParts/colorSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace interpreterBase::robotModel;

WaitForColorSensorBlockBase::WaitForColorSensorBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

DeviceInfo WaitForColorSensorBlockBase::device() const
{
	return DeviceInfo::create<robotParts::ColorSensor>();
}
