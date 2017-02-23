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

#include "umlCheckerPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrutils/xmlUtils.h>
#include <qrutils/outFile.h>
#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace umlChecker;
using namespace utils;

UmlCheckerPlugin::UmlCheckerPlugin()
{
}

UmlCheckerPlugin::~UmlCheckerPlugin()
{
}

void UmlCheckerPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mLogicalModelApi = &configurator.logicalModelApi();
	mGraphicalModelApi = &configurator.graphicalModelApi();

	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
			configurator.mainWindowInterpretersInterface().errorReporter()
			, &configurator.mainWindowInterpretersInterface());
	mRepoControlIFace = &configurator.repoControlInterface();
	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation").toString();

}

QPair<QString, gui::PreferencesPage *> UmlCheckerPlugin::preferencesPage()
{
	return qMakePair(tr(""), nullptr);
}

QList<qReal::ActionInfo> UmlCheckerPlugin::actions()
{
	mUmlCheckerMenu = new QMenu(tr("UmlChecker"));
	ActionInfo umlCheckerMenuInfo(mUmlCheckerMenu, "tools");



	mSaveAction = new QAction(tr("Save Perfect Solution"), nullptr);
	connect(mSaveAction, SIGNAL(triggered()), this, SLOT(saveSolution()));
	mUmlCheckerMenu->addAction(mSaveAction);

	mActionInfos << umlCheckerMenuInfo;

	return mActionInfos;
}



void UmlCheckerPlugin::saveSolution()
{
	IdList const childrenIDs = mLogicalModelApi->children(Id::rootId());
	QHash<QString, IdList> diagramIds;
	foreach (Id const &childId, childrenIDs) {
		if (childId.element() == "RefactoringDiagramNode" && childId == mMainWindowIFace->activeDiagram()) {
			QString elementName = mGraphicalModelApi->name(childId).replace(" ", "")
					.replace("(", "").replace(")", "");
			if (!elementName.isEmpty()) {
				QString fileName = "" + elementName;
				IdList list;
				list << childId;
				diagramIds.insert(fileName + ".qrs", list);
				mMainWindowIFace->saveDiagramAsAPictureToFile(fileName + ".png");
			}
		}
	}
	mRepoControlIFace->saveDiagramsById(diagramIds);
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}
