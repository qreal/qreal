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

BlockInterface *BlocksFactoryManager::block(qReal::Id const &element, RobotModelInterface const &robotModel)
{
	if (!enabledBlocks(robotModel).contains(element.type())) {
		return nullptr;
	}

	for (BlocksFactoryInterface * const factory : factoriesFor(robotModel)) {
		BlockInterface * const block = factory->block(element);
		if (block) {
			return block;
		}
	}

	return nullptr;
}

QSet<qReal::Id> BlocksFactoryManager::enabledBlocks(RobotModelInterface const &robotModel) const
{
	QSet<qReal::Id> result;

	for (blocksBase::BlocksFactoryInterface const *factory : factoriesFor(robotModel)) {
		result += factory->providedBlocks().toSet();
	}

	// The order is important for avoiding collisions cases
	// (we cannot just move this loop body into the previous one)
	for (blocksBase::BlocksFactoryInterface const *factory : factoriesFor(robotModel)) {
		result -= factory->blocksToDisable().toSet();
	}

	return result;
}
