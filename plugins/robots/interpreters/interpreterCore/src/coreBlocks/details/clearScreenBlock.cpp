#include "clearScreenBlock.h"

#include <interpreterBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

ClearScreenBlock::ClearScreenBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void ClearScreenBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	display.clearScreen();
	emit done(mNextBlockId);
}
