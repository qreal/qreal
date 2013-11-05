#include "drawLineBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

DrawLineBlock::DrawLineBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void DrawLineBlock::run()
{
//	int const x1 = evaluate("X1CoordinateLine").toInt();
//	int const y1 = evaluate("Y1CoordinateLine").toInt();
//	int const x2 = evaluate("X2CoordinateLine").toInt();
//	int const y2 = evaluate("Y2CoordinateLine").toInt();
//	mDisplay.drawLine(x1, y1, x2, y2);
	emit done(mNextBlock);
}

QString DrawLineBlock::name() const
{
	return tr("Display");
}
