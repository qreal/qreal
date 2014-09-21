#include "functionBlock.h"

using namespace interpreterCore::coreBlocks::details;

void FunctionBlock::run()
{
	eval("Body");
	if (!errorsOccured()) {
		emit done(mNextBlockId);
	}
}
