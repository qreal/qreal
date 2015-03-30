#include "kitBase/blocksBase/common/waitForColorSensorBlockBase.h"

#include "kitBase/robotModel/robotParts/colorSensorPassive.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace kitBase::robotModel;

WaitForColorSensorBlockBase::WaitForColorSensorBlockBase(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

DeviceInfo WaitForColorSensorBlockBase::device() const
{
	return DeviceInfo::create<robotParts::ColorSensorPassive>();
}
