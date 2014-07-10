#include "interpreterBase/blocksBase/common/emptyBlock.h"

using namespace interpreterBase::blocksBase::common;

void EmptyBlock::run()
{
	emit done(mNextBlockId);
}
