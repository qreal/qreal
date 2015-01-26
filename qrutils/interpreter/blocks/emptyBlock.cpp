#include "emptyBlock.h"

using namespace qReal::interpretation::blocks;

void EmptyBlock::run()
{
	emit done(mNextBlockId);
}
