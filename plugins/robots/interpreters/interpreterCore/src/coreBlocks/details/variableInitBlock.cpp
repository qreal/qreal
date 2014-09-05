#include "variableInitBlock.h"

using namespace interpreterCore::coreBlocks::details;

void VariableInitBlock::run()
{
	evalCode(stringProperty("variable") + " = " + stringProperty("value"));
	if (!wereParserErrors()) {
		emit done(mNextBlockId);
	}
}
