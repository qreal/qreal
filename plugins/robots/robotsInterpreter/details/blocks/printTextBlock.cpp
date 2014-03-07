#include "printTextBlock.h"
#include "../robotParts/robotModel.h"
#include "details/textExpressionProcessor.h"

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
	QString const rawText = stringProperty("PrintText");
	QString const text = TextExpressionProcessor(mParser->variables()).processExpression(rawText);

	mDisplay.printText(x, y, text);
	emit done(mNextBlock);
}

QString PrintTextBlock::name() const
{
	return tr("Display");
}
