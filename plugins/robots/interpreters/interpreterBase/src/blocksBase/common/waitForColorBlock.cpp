#include "interpreterBase/blocksBase/common/waitForColorBlock.h"

using namespace interpreterBase;
using namespace blocksBase::common;

WaitForColorBlock::WaitForColorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

void WaitForColorBlock::responseSlot(int reading)
{
	QString const targetColor = stringProperty("Color");
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
