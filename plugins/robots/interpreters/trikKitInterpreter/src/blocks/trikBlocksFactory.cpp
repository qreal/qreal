#include "trikBlocksFactory.h"

using namespace trikKitInterpreter::blocks;

interpreterBase::blocksBase::Block *TrikBlocksFactory::produceBlock(qReal::Id const &element)
{
	Q_UNUSED(element)
	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	return {};
}
