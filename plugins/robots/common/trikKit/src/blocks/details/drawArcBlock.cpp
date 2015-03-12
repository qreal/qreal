#include "drawArcBlock.h"

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;

DrawArcBlock::DrawArcBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawArcBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	const int x = eval<int>("XCoordinateArc");
	const int y = eval<int>("YCoordinateArc");
	const int width = eval<int>("WidthArc");
	const int height = eval<int>("HeightArc");

	/// Multiplying on 16 needed because Qt require argument in 1/16 degree format.
	const int startAngle = eval<int>("StartAngle") * 16;
	const int spanAngle = eval<int>("SpanAngle") * 16;
	if (!errorsOccured()) {
		trikDisplay->drawArc(x, y, width, height, startAngle, spanAngle);
		emit done(mNextBlockId);
	}
}
