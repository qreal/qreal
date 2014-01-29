#include "finalBlock.h"

using namespace interpreterBase::baseBlocks::details;

void FinalBlock::run()
{
	emit done(qReal::Id());
}

bool FinalBlock::initNextBlocks()
{
	return true;
}
