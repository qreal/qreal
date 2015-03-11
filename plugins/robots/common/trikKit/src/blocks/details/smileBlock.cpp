#include "smileBlock.h"

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik;
using namespace blocks::details;

SmileBlock::SmileBlock(kitBase::robotModel::RobotModelInterface &robotModel, bool isSmileSad)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
	, mIsSmileSad(isSmileSad)
{
}

void SmileBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->drawSmile(mIsSmileSad);
	emit done(mNextBlockId);
}
