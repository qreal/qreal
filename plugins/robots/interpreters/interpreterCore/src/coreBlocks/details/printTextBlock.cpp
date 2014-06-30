#include "printTextBlock.h"

#include <utils/textExpressionProcessor.h>
#include <interpreterBase/robotModel/robotModelUtils.h>
#include <interpreterBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

PrintTextBlock::PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void PrintTextBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	int const x = evaluate("XCoordinateText").toInt();
	int const y = evaluate("YCoordinateText").toInt();
	QString const rawText = stringProperty("PrintText");
	QString const text = utils::TextExpressionProcessor(mParser->variables()).processExpression(rawText);

	display.printText(x, y, text);
	emit done(mNextBlockId);
}
