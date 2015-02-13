#include "functionBlock.h"

using namespace qReal::interpretation::blocks;

void FunctionBlock::run()
{
	eval("Body");
	if (!errorsOccured()) {
		emit done(mNextBlockId);
	}
}
