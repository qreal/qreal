#include "blocksFactoryManager.h"

using namespace interpreterCore;
using namespace interpreterBase;
using namespace blocksBase;
using namespace robotModel;

BlocksFactoryManager::~BlocksFactoryManager()
{
	qDeleteAll(mFactories.values().toSet());
}

void BlocksFactoryManager::addFactory(BlocksFactoryInterface * const factory, RobotModelInterface const *robotModel)
{
	if (!mFactories.values(robotModel).contains(factory)) {
		mFactories.insertMulti(robotModel, factory);
	}
}

QList<BlocksFactoryInterface *> BlocksFactoryManager::factoriesFor(RobotModelInterface const &robotModel) const
{
	return mFactories.values(nullptr) + mFactories.values(&robotModel);
}

BlockInterface *BlocksFactoryManager::block(qReal::Id const &element)
{
	for (BlocksFactoryInterface * const factory : mFactories.values()) {
		BlockInterface * const block = factory->block(element);
		if (block) {
			return block;
		}
	}

	return nullptr;
}
