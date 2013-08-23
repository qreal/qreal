#include "initialBlock.h"

using namespace robotsInterpreterCore::blocks::details;

using namespace robotsInterpreterCore::robotModel;

InitialBlock::InitialBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void InitialBlock::run()
{
	emit done(mNextBlockId);
}
