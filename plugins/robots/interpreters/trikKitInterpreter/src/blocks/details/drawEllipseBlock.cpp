#include "drawEllipseBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawEllipseBlock::DrawEllipseBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawEllipseBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	int const x = evaluate("XCoordinateEllipse").toInt();
	int const y = evaluate("YCoordinateEllipse").toInt();
	int const width = evaluate("WidthEllipse").toInt();
	int const height = evaluate("HeightEllipse").toInt();
	trikDisplay->drawEllipse(x, y, width, height);
	emit done(mNextBlockId);
}
