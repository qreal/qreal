#include "drawRectBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

DrawRectBlock::DrawRectBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void DrawRectBlock::run()
{
	int const x = evaluate("XCoordinateRect").toInt();
	int const y = evaluate("YCoordinateRect").toInt();
	int const width = evaluate("WidthRect").toInt();
	int const height = evaluate("HeightRect").toInt();
	mDisplay.drawRect(x, y, width, height);
	emit done(mNextBlock);
}

QString DrawRectBlock::name() const
{
	return tr("Display");
}
