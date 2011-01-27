#include "initialBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

InitialBlock::InitialBlock(RobotModel &robotModel)
	: mRobotModel(robotModel)
{
	connect(&mRobotModel, SIGNAL(connected(bool)), this, SLOT(connected(bool)));
}

void InitialBlock::run()
{
	mRobotModel.init();
}

void InitialBlock::connected(bool success)
{
	if (success)
		emit done(mNextBlock);
	else {
		error(tr("Can't connect to a robot."));
		emit failure();
	}
}
