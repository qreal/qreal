#include "initialBlock.h"

using namespace interpreterCore::coreBlocks::details;

using namespace interpreterBase::robotModel;

InitialBlock::InitialBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void InitialBlock::run()
{
	emit done(mNextBlockId);
}
