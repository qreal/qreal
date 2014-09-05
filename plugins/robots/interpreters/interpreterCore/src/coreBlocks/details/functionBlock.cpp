#include "functionBlock.h"

using namespace interpreterCore::coreBlocks::details;

void FunctionBlock::run()
{
	eval<int>("Body");
	if (!wereParserErrors()) {
		emit done(mNextBlockId);
	}
}
