#include "interpreterCore/managers/kitAutoSwitcher.h"

#include <qrkernel/settingsManager.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;
using namespace interpreterBase;
using namespace robotModel;

KitAutoSwitcher::KitAutoSwitcher(qReal::ProjectManagementInterface const &projectManager
		, qReal::LogicalModelAssistInterface const &logicalModel
		, BlocksFactoryManagerInterface const &factoryManager
		, KitPluginManager const &kitPluginManager
		, RobotModelManager &robotModelManager
		, QObject *parent)
	: QObject(parent)
	, mLogicalModel(logicalModel)
	, mFactoryManager(factoryManager)
	, mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
{
	connect(&projectManager, &qReal::ProjectManagementInterface::afterOpen, this, &KitAutoSwitcher::onProjectOpened);
}

void KitAutoSwitcher::onProjectOpened()
{
	const QString selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
	QMap<QString, int> const blocksCount = countKitSpecificBlocks();
	if (!selectedKit.isEmpty() && blocksCount[selectedKit] > 0) {
		// If user opens save that contains blocks specific for this kit we do not want
		// to swith kit even if it contains more blocks specific to other kits.
		return;
	}

	int majority = 0;
	QString majorityKit;

	for (const QString &kit : blocksCount.keys()) {
		if (blocksCount[kit] > majority)  {
			majority = blocksCount[kit];
			majorityKit = kit;
		}
	}

	if (majority > 0 && selectedKit != majorityKit) {
		switchTo(majorityKit);
	}
	// Else save contains only common blocks. Ignoring it.
}

QMap<QString, int> KitAutoSwitcher::countKitSpecificBlocks() const
{
	QMap<QString, int> result;
	QMap<qReal::Id, QString> kitBlocks = kitSpecificBlocks();

	for (const qReal::Id &block : mLogicalModel.children(qReal::Id::rootId())) {
		const QString kit = kitBlocks[block.type()];
		if (!kit.isEmpty()) {
			++result[kit];
		}
	}

	return result;
}

QMap<qReal::Id, QString> KitAutoSwitcher::kitSpecificBlocks() const
{
	QMap<QString, QSet<qReal::Id>> kitsToBlocksMap;
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		QSet<qReal::Id> specificBlocks;
		for (KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			for (RobotModelInterface const *robotModel : kit->robotModels()) {
				specificBlocks += mFactoryManager.enabledBlocks(*robotModel);
			}
		}

		kitsToBlocksMap[kitId] = specificBlocks - mFactoryManager.commonBlocks();
	}

	QMap<qReal::Id, QString> blocksToKitsMap;
	for (const QString &kitId : kitsToBlocksMap.keys()) {
		for (const qReal::Id &id: kitsToBlocksMap[kitId]) {
			blocksToKitsMap[id] = kitId;
		}
	}

	return blocksToKitsMap;
}

void interpreterCore::KitAutoSwitcher::switchTo(const QString &kitId)
{
	if (RobotModelInterface * const robotModel
			= RobotModelUtils::selectedRobotModelFor(mKitPluginManager.kitsById(kitId)))
	{
		mRobotModelManager.setModel(robotModel);
	}
}
