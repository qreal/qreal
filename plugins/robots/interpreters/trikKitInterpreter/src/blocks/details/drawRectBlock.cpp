#include "drawRectBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawRectBlock::DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	const int x = eval<int>("XCoordinateRect");
	const int y = eval<int>("YCoordinateRect");
	const int width = eval<int>("WidthRect");
	const int height = eval<int>("HeightRect");
	if (!errorsOccured()) {
		trikDisplay->drawRect(x, y, width, height);
		emit done(mNextBlockId);
	}
}
