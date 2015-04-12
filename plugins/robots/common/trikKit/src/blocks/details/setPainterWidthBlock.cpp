#include "setPainterWidthBlock.h"
#include "setBackgroundBlock.h"

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;

SetPainterWidthBlock::SetPainterWidthBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetPainterWidthBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	const int width = eval<int>("Width");
	if (!errorsOccured()) {
		trikDisplay->setPainterWidth(width);
		emit done(mNextBlockId);
	}
}
