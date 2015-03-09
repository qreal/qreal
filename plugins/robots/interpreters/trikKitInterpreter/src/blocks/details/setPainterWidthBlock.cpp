#include "setPainterWidthBlock.h"
#include "setBackgroundBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

SetPainterWidthBlock::SetPainterWidthBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetPainterWidthBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	const int width = eval<int>("Width");
	if (!errorsOccured()) {
		trikDisplay->setPainterWidth(width);
		emit done(mNextBlockId);
	}
}
