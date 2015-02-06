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
	KitAutoSwitcher(qReal::ProjectManagementInterface const &projectManager
			, qReal::LogicalModelAssistInterface const &logicalModel
			, BlocksFactoryManagerInterface const &factoryManager
			, KitPluginManager const &kitPluginManager
			, RobotModelManager &robotModelManager
			, QObject *parent = nullptr);

private slots:
	void onProjectOpened();
	void switchTo(QString const &kitId);

	QMap<QString, int> countKitSpecificBlocks() const;
	QMap<qReal::Id, QString> kitSpecificBlocks() const;

private:
	qReal::LogicalModelAssistInterface const &mLogicalModel;
	BlocksFactoryManagerInterface const &mFactoryManager;
	KitPluginManager const &mKitPluginManager;
	RobotModelManager &mRobotModelManager;
};

}
