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

#include "interpreterCore/managers/kitAutoSwitcher.h"

#include <qrkernel/settingsManager.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;
using namespace kitBase;
using namespace robotModel;

KitAutoSwitcher::KitAutoSwitcher(const qReal::ProjectManagementInterface &projectManager
		, qReal::LogicalModelAssistInterface &logicalModel
		, const BlocksFactoryManagerInterface &factoryManager
		, const KitPluginManager &kitPluginManager
		, RobotModelManager &robotModelManager
		, QObject *parent)
	: QObject(parent)
	, mLogicalModel(logicalModel)
	, mFactoryManager(factoryManager)
	, mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
{
	connect(&projectManager, &qReal::ProjectManagementInterface::afterOpen, this, &KitAutoSwitcher::onProjectOpened);
	connect(&projectManager, &qReal::ProjectManagementInterface::afterOpen, [this, &robotModelManager]() {
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation("lastKitId", robotModelManager.model().kitId());
	});
	connect(&robotModelManager, &RobotModelManager::robotModelChanged, [this](RobotModelInterface &model) {
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation("lastKitId", model.kitId());
	});
}

void KitAutoSwitcher::onProjectOpened()
{
	if (tryToRestoreFromMetaInformation()) {
		return;
	}

	const QString selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
	QMap<QString, int> const blocksCount = countKitSpecificBlocks();
	if (!selectedKit.isEmpty() && blocksCount[selectedKit] > 0) {
		// If user opens save that contains blocks specific for this kit we do not want
		// to switсh kit even if it contains more blocks specific to other kits.
		return;
	}

	int majority = 0;
	for (const QString &kit : blocksCount.keys()) {
		majority = qMax(blocksCount[kit], majority);
	}

	if (majority == 0) {
		return;
	}

	int bestPriority = -1;
	QString bestKit;
	for (const QString &kit : blocksCount.keys()) {
		if (blocksCount[kit] == majority && mKitPluginManager.priority(kit) > bestPriority) {
			bestPriority = mKitPluginManager.priority(kit);
			bestKit = kit;
		}
	}

	if (selectedKit != bestKit) {
		switchTo(bestKit);
	}
	// Else save contains only common blocks. Ignoring it.
}

bool KitAutoSwitcher::tryToRestoreFromMetaInformation()
{
	const QString kit = mLogicalModel.logicalRepoApi().metaInformation("lastKitId").toString();
	if (!kit.isEmpty() && mKitPluginManager.kitIds().contains(kit)) {
		return switchTo(kit);
	}

	return false;
}

QMap<QString, int> KitAutoSwitcher::countKitSpecificBlocks() const
{
	QMap<QString, int> result;
	QMultiMap<qReal::Id, QString> kitBlocks = kitSpecificBlocks();

	for (const qReal::Id &block : mLogicalModel.children(qReal::Id::rootId())) {
		for (const QString &kit : kitBlocks.values(block.type())) {
			++result[kit];
		}
	}

	return result;
}

QMultiMap<qReal::Id, QString> KitAutoSwitcher::kitSpecificBlocks() const
{
	QMap<QString, QSet<qReal::Id>> kitsToBlocksMap;
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		QSet<qReal::Id> specificBlocks;
		for (KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			for (const RobotModelInterface *robotModel : kit->robotModels()) {
				specificBlocks += mFactoryManager.enabledBlocks(*robotModel);
			}
		}

		kitsToBlocksMap[kitId] = specificBlocks - mFactoryManager.commonBlocks();
	}

	QMultiMap<qReal::Id, QString> blocksToKitsMap;
	for (const QString &kitId : kitsToBlocksMap.keys()) {
		for (const qReal::Id &id: kitsToBlocksMap[kitId]) {
			blocksToKitsMap.insertMulti(id, kitId);
		}
	}

	return blocksToKitsMap;
}

bool interpreterCore::KitAutoSwitcher::switchTo(const QString &kitId)
{
	if (RobotModelInterface * const robotModel
			= RobotModelUtils::selectedRobotModelFor(mKitPluginManager.kitsById(kitId)))
	{
		mRobotModelManager.setModel(robotModel);
		return true;
	}

	return false;
}
