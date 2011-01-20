#include "initialBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

InitialBlock::InitialBlock(RobotModel &robotModel)
	: mRobotModel(robotModel)
{
	connect(&mRobotModel, SIGNAL(connected()), this, SLOT(connected()));
}

void InitialBlock::run()
{
	mRobotModel.init();
}

void InitialBlock::connected()
{
	emit done(mNextBlock);
}
