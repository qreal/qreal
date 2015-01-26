#include "finalBlock.h"

using namespace qReal::interpretation::blocks;

void FinalBlock::run()
{
	emit done(qReal::Id());
}

bool FinalBlock::initNextBlocks()
{
	return true;
}
