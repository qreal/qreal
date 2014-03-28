#include "waitForColorIntensityBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForColorIntensityBlock::WaitForColorIntensityBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

WaitForColorIntensityBlock::~WaitForColorIntensityBlock()
{
}

void WaitForColorIntensityBlock::responseSlot(int reading)
{
	int const targetIntensity = evaluate("Intensity").toInt();
	processResponce(reading, targetIntensity);
}
