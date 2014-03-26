#include "waitForColorIntensityBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForColorIntensityBlock::WaitForColorIntensityBlock(details::RobotModel * const robotModel)
	// Unchecked for now since it needs one of some different color sensor modes.
	: WaitForColorSensorBlockBase(robotModel, robots::enums::sensorType::unused)
{
}

void WaitForColorIntensityBlock::responseSlot(int reading)
{
	int const targetIntensity = evaluate("Intensity").toInt();
	processResponce(reading, targetIntensity);
}
