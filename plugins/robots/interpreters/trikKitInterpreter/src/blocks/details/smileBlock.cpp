#include "smileBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter;
using namespace blocks::details;

SmileBlock::SmileBlock(interpreterBase::robotModel::RobotModelInterface &robotModel, bool isSmileSad)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
	, mIsSmileSad(isSmileSad)
{
}

void SmileBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->drawSmile(mIsSmileSad);
	emit done(mNextBlockId);
}
