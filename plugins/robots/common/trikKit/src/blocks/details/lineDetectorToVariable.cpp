#include "lineDetectorToVariable.h"

using namespace trik::blocks::details;

LineDetectorToVariableBlock::LineDetectorToVariableBlock()
{
}

void LineDetectorToVariableBlock::run()
{
	evalCode(stringProperty("Variable") + " = lineSensor[0]");
	emit done(mNextBlockId);
}
