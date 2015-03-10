#include "clearScreenBlock.h"

#include <kitBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

ClearScreenBlock::ClearScreenBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void ClearScreenBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	display.clearScreen();
	emit done(mNextBlockId);
}
