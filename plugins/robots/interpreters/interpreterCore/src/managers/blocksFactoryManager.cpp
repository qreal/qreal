#include "interpreterCore/managers/blocksFactoryManager.h"

#include <qrutils/interpreter/blocks/emptyBlock.h>

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

qReal::interpretation::BlockInterface *BlocksFactoryManager::block(qReal::Id const &element
		, RobotModelInterface const &robotModel)
{
	qReal::interpretation::BlockInterface *emptyBlock = nullptr;

	for (BlocksFactoryInterface * const factory : factoriesFor(robotModel)) {
		qReal::interpretation::BlockInterface * const block = factory->block(element);
		if (block && !dynamic_cast<qReal::interpretation::blocks::EmptyBlock *>(block)) {
			return block;
		} else {
			/// @todo: Ask for empty block somewhere else, not memorizing it here.
			emptyBlock = block;
		}
	}

	return emptyBlock;
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

QSet<qReal::Id> BlocksFactoryManager::visibleBlocks(RobotModelInterface const &robotModel) const
{
	QSet<qReal::Id> result;

	QString const kitId = robotModel.kitId();

	for (RobotModelInterface const *robotModel : mFactories.keys()) {
		if (robotModel && robotModel->kitId() == kitId) {
			for (BlocksFactoryInterface const * factory : factoriesFor(*robotModel)) {
				result += factory->providedBlocks().toSet();
			}
		}
	}

	return result;
}

QSet<qReal::Id> BlocksFactoryManager::commonBlocks() const
{
	QSet<qReal::Id> result;
	for (BlocksFactoryInterface const *factory : mFactories.values(nullptr)) {
		result += factory->providedBlocks().toSet();
	}

	return result;
}

QList<BlocksFactoryInterface *> BlocksFactoryManager::factoriesFor(RobotModelInterface const &robotModel) const
{
	return mFactories.values(nullptr) + mFactories.values(&robotModel);
}
