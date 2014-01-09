#include "finalBlock.h"

using namespace interpreterBase::blocks::details;

void FinalBlock::run()
{
	emit done(qReal::Id());
}

bool FinalBlock::initNextBlocks()
{
	return true;
}
