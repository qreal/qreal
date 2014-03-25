#include "drawRectBlock.h"

using namespace nxtKitInterpreter::blocks::details;

DrawRectBlock::DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	int const x = evaluate("XCoordinateRect").toInt();
	int const y = evaluate("YCoordinateRect").toInt();
	int const width = evaluate("WidthRect").toInt();
	int const height = evaluate("HeightRect").toInt();
	display.drawRect(x, y, width, height);
	emit done(mNextBlockId);
}
