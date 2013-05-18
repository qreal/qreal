#include "printTextBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

PrintTextBlock::PrintTextBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void PrintTextBlock::run()
{
	int const x = evaluate("XCoordinateText").toInt();
	int const y = evaluate("YCoordinateText").toInt();
	//QString const text = evaluate("PrintText").toString();
	QString text = "no text";

	mDisplay.printText(x, y, text);
	emit done(mNextBlock);
}

QString PrintTextBlock::name() const
{
	return tr("Display");
}
