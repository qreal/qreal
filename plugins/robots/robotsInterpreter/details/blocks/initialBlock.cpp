#include "initialBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

InitialBlock::InitialBlock(details::RobotModel &robotModel)
	: mRobotModel(robotModel)
{
}

void InitialBlock::run()
{
	emit done(mNextBlock);
}
