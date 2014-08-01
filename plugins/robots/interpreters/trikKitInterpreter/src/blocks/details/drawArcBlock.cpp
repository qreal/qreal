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
	int const x = evaluate("XCoordinateArc").toInt();
	int const y = evaluate("YCoordinateArc").toInt();
	int const width = evaluate("WidthArc").toInt();
	int const height = evaluate("HeightArc").toInt();
	int const startAngle = evaluate("StartAngle").toInt();
	int const spanAngle = evaluate("SpanAngle").toInt();
	trikDisplay->drawArc(x, y, width, height, startAngle, spanAngle);
	emit done(mNextBlockId);
}
