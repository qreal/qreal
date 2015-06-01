/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreterCore/managers/blocksFactoryManager.h"

#include <qrutils/interpreter/blocks/emptyBlock.h>

using namespace interpreterCore;
using namespace kitBase;
using namespace blocksBase;
using namespace robotModel;

BlocksFactoryManager::~BlocksFactoryManager()
{
	qDeleteAll(mFactories.values().toSet());
}

void BlocksFactoryManager::addFactory(BlocksFactoryInterface * const factory, const RobotModelInterface *robotModel)
{
	if (!mFactories.values(robotModel).contains(factory)) {
		mFactories.insertMulti(robotModel, factory);
	}
}

qReal::interpretation::BlockInterface *BlocksFactoryManager::block(const qReal::Id &element
		, const RobotModelInterface &robotModel)
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

QSet<qReal::Id> BlocksFactoryManager::enabledBlocks(const RobotModelInterface &robotModel) const
{
	QSet<qReal::Id> result;

	for (const blocksBase::BlocksFactoryInterface *factory : factoriesFor(robotModel)) {
		result += factory->providedBlocks().toSet();
	}

	// The order is important for avoiding collisions cases
	// (we cannot just move this loop body into the previous one)
	for (const blocksBase::BlocksFactoryInterface *factory : factoriesFor(robotModel)) {
		result -= factory->blocksToDisable().toSet();
	}

	return result;
}

QSet<qReal::Id> BlocksFactoryManager::visibleBlocks(const RobotModelInterface &robotModel) const
{
	QSet<qReal::Id> result;

	const QString kitId = robotModel.kitId();

	for (const RobotModelInterface *robotModel : mFactories.keys()) {
		if (robotModel && robotModel->kitId() == kitId) {
			for (const BlocksFactoryInterface * factory : factoriesFor(*robotModel)) {
				result += factory->providedBlocks().toSet();
			}
		}
	}

	return result;
}

QSet<qReal::Id> BlocksFactoryManager::commonBlocks() const
{
	QSet<qReal::Id> result;
	for (const BlocksFactoryInterface *factory : mFactories.values(nullptr)) {
		result += factory->providedBlocks().toSet();
	}

	return result;
}

QList<BlocksFactoryInterface *> BlocksFactoryManager::factoriesFor(const RobotModelInterface &robotModel) const
{
	return mFactories.values(nullptr) + mFactories.values(&robotModel);
}
