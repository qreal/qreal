#include "drawCircleBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

DrawCircleBlock::DrawCircleBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void DrawCircleBlock::run()
{
	int const x = evaluate("XCoordinateCircle").toInt();
	int const y = evaluate("YCoordinateCircle").toInt();
	int const radius = evaluate("CircleRadius").toInt();
	mDisplay.drawCircle(x, y, radius);
	emit done(mNextBlock);
}

QString DrawCircleBlock::name() const
{
	return tr("Display");
}
