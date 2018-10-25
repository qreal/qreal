/* Copyright 2018 CyberTech Labs Ltd.
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

#include "subprogramsImporterExporterPlugin.h"

using namespace subprogramsImporterExporter;

SubprogramsImporterExporterPlugin::SubprogramsImporterExporterPlugin()
	: mImportToProjectAction(tr("Import subprograms to current project"), nullptr)
	, mExportAction(tr("Export subprograms to file"), nullptr)
	, mRepo(nullptr)
	, mMainWindowInterpretersInterface(nullptr)
	, mGraphicalModel(nullptr)
	, mLogicalModel(nullptr)
	, mProjectManager(nullptr)
{
	connect(&mExportAction, &QAction::triggered, this, &SubprogramsImporterExporterPlugin::exportToFile);
	connect(&mImportToProjectAction, &QAction::triggered, this, &SubprogramsImporterExporterPlugin::importToProject);
}

SubprogramsImporterExporterPlugin::~SubprogramsImporterExporterPlugin()
{
}

QList<qReal::ActionInfo> SubprogramsImporterExporterPlugin::actions()
{
	return { qReal::ActionInfo(&mImportToProjectAction, "", "tools")
			, qReal::ActionInfo(&mExportAction, "", "tools") };
}

void SubprogramsImporterExporterPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mGraphicalModel = &configurator.graphicalModelApi();
	mLogicalModel = &configurator.logicalModelApi();
	mProjectManager = &configurator.projectManager();
}

void SubprogramsImporterExporterPlugin::exportToFile() const
{
	qReal::IdList graphicalChildrens = mGraphicalModel->children(mGraphicalModel->rootId());
	qReal::IdList exportedIds;
	for (auto id : graphicalChildrens) {
		if (id.element() == "Subprogram" || id.element() == "SubprogramDiagram") {
			qReal::IdList childrens = mGraphicalModel->children(id);
			exportedIds.append(childrens);
			exportedIds.append(id);
		}
	}

	qReal::IdList logicalChildrens = mLogicalModel->children(mGraphicalModel->rootId());
	for (auto id : logicalChildrens) {
		if (id.element() == "Subprogram" || id.element() == "SubprogramDiagram") {
			qReal::IdList childrens = mLogicalModel->children(id);
			exportedIds.append(childrens);
			exportedIds.append(id);
		}

		qReal::IdList localIncomingExplosions = mLogicalModel->logicalRepoApi().incomingExplosions(id);
		if (not localIncomingExplosions.isEmpty()) {
			exportedIds.append(localIncomingExplosions);
		}
	}

	QSet<qReal::Id> set;
	for (auto id : exportedIds) {
		set.insert(id);
		if (mGraphicalModel->graphicalRepoApi().isGraphicalElement(id)) {
			qReal::Id logicalId = mGraphicalModel->graphicalRepoApi().logicalId(id);
			if (not logicalId.isNull()) {
				set.insert(logicalId);
			}
		}
	}

	QString currnetWorkingFile = mRepo->workingFile();
	mRepo->setWorkingFile("/home/greg/Documents/qrealFolder/importTest1.qrs");
	mRepo->save(set.toList());
	mRepo->setWorkingFile(currnetWorkingFile);
}

void SubprogramsImporterExporterPlugin::importToProject() const
{
	qReal::Id activeDiagram = mMainWindowInterpretersInterface->activeDiagram();
	mRepo->importFromDisk("/home/greg/Documents/qrealFolder/importTest1.qrs");
	mMainWindowInterpretersInterface->reinitModels();
	mMainWindowInterpretersInterface->activateItemOrDiagram(activeDiagram);
	mProjectManager->afterOpen(mRepo->workingFile());
}
