#include "initialBlock.h"

using namespace interpreterBase::blocks::details;

using namespace interpreterBase::robotModel;

InitialBlock::InitialBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void InitialBlock::run()
{
	emit done(mNextBlockId);
}
