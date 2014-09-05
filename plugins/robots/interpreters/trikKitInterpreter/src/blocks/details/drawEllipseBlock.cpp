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
	int const x = eval<int>("XCoordinateEllipse");
	int const y = eval<int>("YCoordinateEllipse");
	int const width = eval<int>("WidthEllipse");
	int const height = eval<int>("HeightEllipse");
	if (!wereParserErrors()) {
		trikDisplay->drawEllipse(x, y, width, height);
		emit done(mNextBlockId);
	}
}
