#include "drawPixelBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

DrawPixelBlock::DrawPixelBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void DrawPixelBlock::run()
{
	int const x = evaluate("XCoordinatePix").toInt();
	int const y = evaluate("YCoordinatePix").toInt();
//	mDisplay.drawPixel(x, y);
	emit done(mNextBlock);
}

QString DrawPixelBlock::name() const
{
	return tr("Display");
}
