#include "finalBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

void FinalBlock::run()
{
	emit done(NULL);
}

bool FinalBlock::initNextBlocks()
{
	return true;
}
