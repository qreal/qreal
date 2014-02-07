#include "blocksFactoryManager.h"

using namespace interpreterCore;
using namespace interpreterBase::blocksBase;

void BlocksFactoryManager::addFactory(BlocksFactoryInterface * const factory)
{
	mFactories << QSharedPointer<BlocksFactoryInterface>(factory);
}

BlockInterface *BlocksFactoryManager::block(qReal::Id const &element)
{
	for (QSharedPointer<BlocksFactoryInterface> const &factory : mFactories) {
		BlockInterface * const block = factory->block(element);
		if (block) {
			return block;
		}
	}

	return nullptr;
}

qReal::IdList BlocksFactoryManager::providedBlocks() const
{
	qReal::IdList result;

	for (QSharedPointer<BlocksFactoryInterface> const &factory : mFactories) {
		result << factory->providedBlocks();
	}

	return result;
}
