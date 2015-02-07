#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>

#include "blocksFactoryManagerInterface.h"
#include "kitPluginManager.h"
#include "robotModelManager.h"

namespace interpreterCore {

/// Perfroms auto-switching of selected kit when user opens saves.
/// Switching to other kit happens if and only if the save contains at least one block
/// specific to that kit only and no blocks for current kit.
class KitAutoSwitcher : public QObject
{
	Q_OBJECT

public:
	KitAutoSwitcher(const qReal::ProjectManagementInterface &projectManager
			, const qReal::LogicalModelAssistInterface &logicalModel
			, const BlocksFactoryManagerInterface &factoryManager
			, const KitPluginManager &kitPluginManager
			, RobotModelManager &robotModelManager
			, QObject *parent = nullptr);

private slots:
	void onProjectOpened();
	void switchTo(const QString &kitId);

	QMap<QString, int> countKitSpecificBlocks() const;
	QMap<qReal::Id, QString> kitSpecificBlocks() const;

private:
	const qReal::LogicalModelAssistInterface &mLogicalModel;
	const BlocksFactoryManagerInterface &mFactoryManager;
	const KitPluginManager &mKitPluginManager;
	RobotModelManager &mRobotModelManager;
};

}
