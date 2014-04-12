#include "waitForColorSensorBlockBase.h"

#include <interpreterBase/robotModel/robotParts/colorSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForColorSensorBlockBase::WaitForColorSensorBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForColorSensorBlockBase::~WaitForColorSensorBlockBase()
{
}

DeviceInfo WaitForColorSensorBlockBase::device() const
{
	return DeviceInfo::create<robotParts::ColorSensor>();
}
