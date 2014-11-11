#include "initialBlock.h"

using namespace interpreterCore::coreBlocks::details;

using namespace interpreterBase::robotModel;

void InitialBlock::run()
{
	emit done(mNextBlockId);
}
