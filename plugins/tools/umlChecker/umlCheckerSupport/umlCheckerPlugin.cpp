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
	: mTemplatesWindow(nullptr)
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
	mRepoControlIFace = &configurator.repoControlInterface();

	mTemplatesWindow = new UmlCheckerTmplWindow(mMainWindowIFace->windowWidget());
	connect(mTemplatesWindow, SIGNAL(applyButtonClicked()), this, SLOT(save()));
	connect(mTemplatesWindow, SIGNAL(applyButtonClicked()), this, SLOT(cancel()));

	mOrdinaryRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/ordinary", true);
	mPerfectRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/perfect", true);

	mHandler = new UmlCheckerHandler(mPerfectRepoApi, mOrdinaryRepoApi);
}

QPair<QString, gui::PreferencesPage *> UmlCheckerPlugin::preferencesPage()
{
	return qMakePair(tr("UmlChecker"), nullptr);
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

	mOpenTemplatesWindowAction = new QAction(tr("Open Templates Window"), nullptr);
	connect(mOpenTemplatesWindowAction, SIGNAL(triggered()), this, SLOT(openTemplatesWindow()));
	mUmlCheckerMenu->addAction(mOpenTemplatesWindowAction);

	mAssignTemplates = new QAction(tr("Assign Templates"), nullptr);
	connect(mAssignTemplates, SIGNAL(triggered()), this, SLOT(assignTemplatesForBlock()));
	mUmlCheckerMenu->addAction(mAssignTemplates);

	mSaveTemplate = new QAction(tr("Save Template"), nullptr);
	connect(mSaveTemplate, SIGNAL(triggered()), this, SLOT(saveTemplate()));
	mUmlCheckerMenu->addAction(mSaveTemplate);

	mActionInfos << umlCheckerMenuInfo;

	return mActionInfos;
}

void UmlCheckerPlugin::save()
{
	qDebug() << "save";
}


void UmlCheckerPlugin::cancel()
{
	qDebug() << "cancel";
}

void UmlCheckerPlugin::saveTemplate()
{
	const QString tempDirPath = "/home/julia/qreal/qreal/plugins/tools/umlChecker/test/";
	mMainWindowIFace->saveDiagramAsAPictureToFile(tempDirPath + "temp.png");
	mRepoControlIFace->saveTo(tempDirPath + "temp.qrs");
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
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

void UmlCheckerPlugin::openTemplatesWindow()
{
	mTemplatesWindow->show();
	mTemplatesWindow->openTemplatesForBlocks();
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
