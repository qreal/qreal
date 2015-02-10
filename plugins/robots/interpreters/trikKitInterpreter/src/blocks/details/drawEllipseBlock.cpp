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
	const int x = eval<int>("XCoordinateEllipse");
	const int y = eval<int>("YCoordinateEllipse");
	const int width = eval<int>("WidthEllipse");
	const int height = eval<int>("HeightEllipse");
	if (!errorsOccured()) {
		trikDisplay->drawEllipse(x, y, width, height);
		emit done(mNextBlockId);
	}
}
