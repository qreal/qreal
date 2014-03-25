#include "clearScreenBlock.h"

using namespace nxtKitInterpreter::blocks::details;

ClearScreenBlock::ClearScreenBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void ClearScreenBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	display.clearScreen();
	emit done(mNextBlockId);
}
