#include "engineCommandBlock.h"

using namespace interpreterCore::coreBlocks::details;

EngineCommandBlock::EngineCommandBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void EngineCommandBlock::timeout()
{
	emit done(mNextBlockId);
}
