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

#include "kitBase/blocksBase/commonBlocksFactory.h"

#include <kitBase/blocksBase/robotsBlock.h>
#include <qrutils/interpreter/blocks/emptyBlock.h>

using namespace kitBase::blocksBase;

CommonBlocksFactory::CommonBlocksFactory()
	: mRobotModelManager(nullptr)
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mErrorReporter(nullptr)
	, mParser(nullptr)
{
}

qReal::interpretation::BlockInterface *CommonBlocksFactory::block(const qReal::Id &element)
{
	qReal::interpretation::Block *newBlock = blocksToDisable().contains(element.type())
			? new qReal::interpretation::blocks::EmptyBlock
			: produceBlock(element);

	if (!newBlock) {
		newBlock = new qReal::interpretation::blocks::EmptyBlock;
	}

	if (RobotsBlock * const robotsBlock = dynamic_cast<RobotsBlock *>(newBlock)) {
		robotsBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi
				, mErrorReporter, *mRobotModelManager, *mParser);
	} else {
		newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, *mParser);
	}

	return newBlock;
}

void CommonBlocksFactory::configure(const qReal::GraphicalModelAssistInterface &graphicalModelApi
		, const qReal::LogicalModelAssistInterface &logicalModelApi
		, kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
	mParser = &textLanguageToolbox;
}

qReal::IdList CommonBlocksFactory::blocksToDisable() const
{
	return {};
}

qReal::Id CommonBlocksFactory::id(const QString &metatype)
{
	return qReal::Id("RobotsMetamodel", "RobotsDiagram", metatype);
}

bool CommonBlocksFactory::elementMetatypeIs(const qReal::Id &element, const QString &metatype)
{
	return element.type() == id(metatype);
}
