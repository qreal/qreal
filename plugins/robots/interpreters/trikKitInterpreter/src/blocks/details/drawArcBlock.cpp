#include "drawArcBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawArcBlock::DrawArcBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawArcBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
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
