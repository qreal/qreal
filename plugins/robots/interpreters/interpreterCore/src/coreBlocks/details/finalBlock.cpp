#include "finalBlock.h"

using namespace interpreterCore::coreBlocks::details;

void FinalBlock::run()
{
	emit done(qReal::Id());
}

bool FinalBlock::initNextBlocks()
{
	return true;
}
