#include "initialBlock.h"

using namespace qReal::interpretation::blocks;

void InitialBlock::run()
{
	emit done(mNextBlockId);
}
