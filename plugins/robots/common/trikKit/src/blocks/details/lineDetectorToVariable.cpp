#include "lineDetectorToVariable.h"

using namespace trik::blocks::details;

LineDetectorToVariableBlock::LineDetectorToVariableBlock()
{
}

void LineDetectorToVariableBlock::run()
{
	evalCode(stringProperty("variable") + " = lineSensor[0]");
	emit done(mNextBlockId);
}
