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
	int const startAngle = eval<int>("StartAngle");
	int const spanAngle = eval<int>("SpanAngle");
	if (!wereParserErrors()) {
		trikDisplay->drawArc(x, y, width, height, startAngle, spanAngle);
		emit done(mNextBlockId);
	}
}
