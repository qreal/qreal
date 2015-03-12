#include "drawEllipseBlock.h"

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;

DrawEllipseBlock::DrawEllipseBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawEllipseBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	const int x = eval<int>("XCoordinateEllipse");
	const int y = eval<int>("YCoordinateEllipse");
	const int width = eval<int>("WidthEllipse");
	const int height = eval<int>("HeightEllipse");
	if (!errorsOccured()) {
		trikDisplay->drawEllipse(x, y, width, height);
		emit done(mNextBlockId);
	}
}
