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
	int const x = eval<int>("XCoordinateArc");
	int const y = eval<int>("YCoordinateArc");
	int const width = eval<int>("WidthArc");
	int const height = eval<int>("HeightArc");

	/// Multiplying on 16 needed because Qt require argument in 1/16 degree format.
	int const startAngle = eval<int>("StartAngle") * 16;
	int const spanAngle = eval<int>("SpanAngle") * 16;
	if (!errorsOccured()) {
		trikDisplay->drawArc(x, y, width, height, startAngle, spanAngle);
		emit done(mNextBlockId);
	}
}
