#include "printTextBlock.h"
#include <utils/textExpressionProcessor.h>

using namespace nxtKitInterpreter::blocks::details;

PrintTextBlock::PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void PrintTextBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	int const x = evaluate("XCoordinateText").toInt();
	int const y = evaluate("YCoordinateText").toInt();
	QString const rawText = stringProperty("PrintText");
	QString const text = utils::TextExpressionProcessor(mParser->variables()).processExpression(rawText);

	display.printText(x, y, text);
	emit done(mNextBlockId);
}
