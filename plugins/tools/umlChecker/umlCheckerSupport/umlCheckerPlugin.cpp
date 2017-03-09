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
	mQRealSourceFilesPath = "/home/julia/qreal/qreal";
	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();

//	mTemplatesWindow = new UmlCheckerTmplWindow(mMainWindowIFace->windowWidget());

	mOrdinaryRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/perfect", true);
	mPerfectRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/ordinary", true);

	mHandler = new UmlCheckerHandler(mPerfectRepoApi, mOrdinaryRepoApi);
}

QPair<QString, gui::PreferencesPage *> UmlCheckerPlugin::preferencesPage()
{
	return qMakePair(tr(""), nullptr);
}

QList<qReal::ActionInfo> UmlCheckerPlugin::actions()
{
	mUmlCheckerMenu = new QMenu(tr("UmlChecker"));
	ActionInfo umlCheckerMenuInfo(mUmlCheckerMenu, "tools");

	mParseAction = new QAction(tr("Parse Solution"), nullptr);
	connect(mParseAction, SIGNAL(triggered()), this, SLOT(parseSolution()));
	mUmlCheckerMenu->addAction(mParseAction);

	mAddBlockAction = new QAction(tr("Add Block"), nullptr);
	connect(mAddBlockAction, SIGNAL(triggered()), this, SLOT(addElementsToBlock()));
	mUmlCheckerMenu->addAction(mAddBlockAction);

	mSavePerfectSolution = new QAction(tr("Save Perfect Solution"), nullptr);
	connect(mSavePerfectSolution, SIGNAL(triggered()), this, SLOT(savePerfectSolution()));
	mUmlCheckerMenu->addAction(mSavePerfectSolution);

	mAssignTemplates = new QAction(tr("Assign Templates"), nullptr);
	connect(mAssignTemplates, SIGNAL(triggered()), this, SLOT(assignTemplatesForBlock()));
	mUmlCheckerMenu->addAction(mAssignTemplates);

	mActionInfos << umlCheckerMenuInfo;

	return mActionInfos;
}

void UmlCheckerPlugin::addElementsToBlock()
{
	const QString blockName = QInputDialog::getText(nullptr, tr("block name"), tr("enter block name"));
	IdList activeElements = mMainWindowIFace->selectedElementsOnActiveDiagram();
	for (Id &id : activeElements) {
		mPerfectRepoApi->setProperty(id, blockName, QVariant(""));
	}

	mHandler->addBlockName(blockName);
}


void UmlCheckerPlugin::assignTemplatesForBlock()
{

}

void UmlCheckerPlugin::savePerfectSolution()
{
	mHandler->saveSolution();
}


void UmlCheckerPlugin::parseSolution()
{
	bool matchingResult = mHandler->matchingResult();

	if (matchingResult) {
		QMessageBox::information(nullptr, tr("Information"), tr("Successfully"), tr("Ok"));
	} else {
		QMessageBox::information(nullptr, tr("Information"), tr("Fail"), tr("Ok"));
	}
}
