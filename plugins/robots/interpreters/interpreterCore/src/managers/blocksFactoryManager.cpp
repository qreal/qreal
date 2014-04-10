#include "blocksFactoryManager.h"

using namespace interpreterCore;
using namespace interpreterBase::blocksBase;

BlocksFactoryManager::~BlocksFactoryManager()
{
	qDeleteAll(mFactories);
}

void BlocksFactoryManager::addFactory(BlocksFactoryInterface * const factory)
{
	if (!mFactories.contains(factory)) {
		mFactories << factory;
	}
}

BlockInterface *BlocksFactoryManager::block(qReal::Id const &element)
{
	for (BlocksFactoryInterface * const factory : mFactories) {
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

	for (BlocksFactoryInterface * const factory : mFactories) {
		result << factory->providedBlocks();
	}

	return result;
}
