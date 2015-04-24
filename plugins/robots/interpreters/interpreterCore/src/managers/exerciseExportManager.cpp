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

#include "exerciseExportManager.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QUuid>

#include <qrgui/models/logicalModelAssistApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrrepo/repoControlInterface.h>
#include <qrutils/qRealFileDialog.h>

#include "src/ui/exerciseExportDialog.h"

using namespace interpreterCore;
using namespace kitBase;

ExerciseExportManager::ExerciseExportManager(qReal::LogicalModelAssistInterface &logicalModel
		, qrRepo::RepoControlInterface &repoControlApi, qReal::ProjectManagementInterface &projectManager)
	: mLogicalModel(logicalModel)
	, mRepoControlApi(repoControlApi)
	, mProjectManager(projectManager)
{
}

ExerciseExportManager::~ExerciseExportManager()
{
}

void ExerciseExportManager::save()
{
	ui::ExerciseExportDialog dialog;
	if (dialog.exec() == QDialog::Rejected) {
		return;
	}

	ReadOnlyFlags flags = dialog.readOnlyFlags();

	const auto save = [this, &flags] (const QString &tag, ReadOnly::ReadOnlyEnum flag) {
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation(tag, flags.testFlag(flag));
	};

	save("twoDModelWorldReadOnly", ReadOnly::World);
	save("twoDModelSensorsReadOnly", ReadOnly::Sensors);
	save("twoDModelRobotPositionReadOnly", ReadOnly::RobotPosition);
	save("twoDModelRobotConfigurationReadOnly", ReadOnly::RobotSetup);
	save("twoDModelSimulationSettingsReadOnly", ReadOnly::SimulationSettings);

	mLogicalModel.mutableLogicalRepoApi().setMetaInformation("exerciseId", QUuid::createUuid().toString());

	QString fileName = utils::QRealFileDialog::getSaveFileName("SaveAsTask"
			, nullptr
			, QObject::tr("Select file to export save to")
			, "."
			, QObject::tr("QReal Save File(*.qrs)"));

	if (fileName.isEmpty()) {
		return;
	}

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}

	mRepoControlApi.saveTo(fileName);

	mProjectManager.open(fileName);
}
