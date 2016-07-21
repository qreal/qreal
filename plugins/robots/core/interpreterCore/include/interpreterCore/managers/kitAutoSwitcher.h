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
			, qReal::LogicalModelAssistInterface &logicalModel
			, const BlocksFactoryManagerInterface &factoryManager
			, const KitPluginManager &kitPluginManager
			, RobotModelManager &robotModelManager
			, QObject *parent = nullptr);

private slots:
	void onProjectOpened();
	bool tryToRestoreFromMetaInformation();
	bool switchTo(const QString &kitId);

	QMap<QString, int> countKitSpecificBlocks() const;
	QMultiMap<qReal::Id, QString> kitSpecificBlocks() const;

private:
	qReal::LogicalModelAssistInterface &mLogicalModel;
	const BlocksFactoryManagerInterface &mFactoryManager;
	const KitPluginManager &mKitPluginManager;
	RobotModelManager &mRobotModelManager;
};

}
