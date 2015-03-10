#include "kitBase/blocksBase/common/waitForColorBlock.h"

#include "kitBase/robotModel/robotParts/colorSensorFull.h"

using namespace kitBase;
using namespace blocksBase::common;

WaitForColorBlock::WaitForColorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

void WaitForColorBlock::responseSlot(int reading)
{
	const QString targetColor = stringProperty("Color");
	QString color;
	switch (reading) {
	case 1: color = tr("Black");
		break;
	case 2: color = tr("Blue");
		break;
	case 3: color = tr("Green");
		break;
	case 4: color = tr("Yellow");
		break;
	case 5: color = tr("Red");
		break;
	case 6: color = tr("White");
		break;
	default:
		return;
	}

	if (targetColor == color) {
		stop();
	}
}

robotModel::DeviceInfo WaitForColorBlock::device() const
{
	return robotModel::DeviceInfo::create<robotModel::robotParts::ColorSensorFull>();
}
