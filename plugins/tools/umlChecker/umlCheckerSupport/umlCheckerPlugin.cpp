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
	, mUmlCheckerTemplate(nullptr)
	, mQRealFilesPath("")
{
}

Customizer * UmlCheckerPlugin::customizationInterface()
{
	return &mCustomizer;
}


UmlCheckerPlugin::~UmlCheckerPlugin()
{
}

void UmlCheckerPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mQRealFilesPath = SettingsManager::value("pathToToolPlugins", ".").toString();

	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();
	mRepoControlIFace = &configurator.repoControlInterface();

	mTemplatesWindow = new UmlCheckerTmplWindow(mMainWindowIFace->windowWidget());
	connect(mTemplatesWindow, SIGNAL(rejected()), this, SLOT(cancel()));

	connect(mTemplatesWindow, SIGNAL(blocksButtonClicked()), this, SLOT(save()));
	connect(mTemplatesWindow, SIGNAL(cancelButtonClicked()), this, SLOT(cancel()));

	mUmlCheckerTemplate = new UmlCheckerTemplate(mMainWindowIFace, mRepoControlIFace);
	mUmlCheckerPerfectSolution = new UmlCheckerPerfectSolution(mMainWindowIFace, mRepoControlIFace);

	mHandler = new UmlCheckerHandler();
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

	mAddEdgesAction = new QAction(tr("Add Edges"), nullptr);
	connect(mAddEdgesAction, SIGNAL(triggered()), this, SLOT(addElementsToEdge()));
	mUmlCheckerMenu->addAction(mAddEdgesAction);

	mSavePerfectSolution = new QAction(tr("Save Perfect Solution"), nullptr);
	connect(mSavePerfectSolution, SIGNAL(triggered()), this, SLOT(savePerfectSolution()));
	mUmlCheckerMenu->addAction(mSavePerfectSolution);

	mOpenTemplatesWindowAction = new QAction(tr("Open Templates Window"), nullptr);
	connect(mOpenTemplatesWindowAction, SIGNAL(triggered()), this, SLOT(openTemplatesWindow(QString)));
	mUmlCheckerMenu->addAction(mOpenTemplatesWindowAction);

	mSaveTemplate = new QAction(tr("Save Template"), nullptr);
	connect(mSaveTemplate, SIGNAL(triggered()), this, SLOT(saveTemplate()));
	mUmlCheckerMenu->addAction(mSaveTemplate);

	mActionInfos << umlCheckerMenuInfo;

	return mActionInfos;
}

void UmlCheckerPlugin::save()
{
	const QPair<QString, QPair<QString, QStringList>> elements = mTemplatesWindow->getElementForBlock();
	if (elements.first == "Edge") {
		mUmlCheckerPerfectSolution->saveOptionsForEdge(elements.second);
	} else {
		mUmlCheckerPerfectSolution->saveOptionsForBlock(elements.second);
	}

	mUmlCheckerPerfectSolution->saveAll();
}

void UmlCheckerPlugin::cancel()
{

}

void UmlCheckerPlugin::saveTemplate()
{
	mUmlCheckerTemplate->saveTemplate();
}

void UmlCheckerPlugin::addElementsToBlock()
{
	const QString blockName = QInputDialog::getText(nullptr, tr("enter block name"), tr("enter block name"));

	mUmlCheckerPerfectSolution->addElementsToBlock(blockName);

	mTemplatesWindow->setBlockName(blockName);
	mTemplatesWindow->setType("Block");

	const QString tempDirPath = mQRealFilesPath + "/templates/";
	openTemplatesWindow(tempDirPath);
}

void UmlCheckerPlugin::addElementsToEdge()
{
	const QString edgeName = QInputDialog::getText(nullptr, tr("enter edge name"), tr("enter edge name"));
	const QString tempDirPath = mQRealFilesPath + "/edge/";
	mTemplatesWindow->setBlockName(edgeName);
	mTemplatesWindow->setType("Edge");

	openTemplatesWindow(tempDirPath);
}

void UmlCheckerPlugin::openTemplatesWindow(const QString &name)
{
	mTemplatesWindow->show();
	mTemplatesWindow->openTemplatesForBlocks(name);
}

void UmlCheckerPlugin::savePerfectSolution()
{
	mUmlCheckerPerfectSolution->saveTempSolution();
}

void UmlCheckerPlugin::parseSolution()
{
	const QString ordinaryPath = mRepoControlIFace->workingFile();
	const QString perfectSolutionsPath = QFileDialog::getExistingDirectory(nullptr, tr("Specify perfect directory:")) + "/";;
	mHandler->init(ordinaryPath, perfectSolutionsPath);
	bool matchingResult = mHandler->matchingResult();
	mHandler->clear();

	if (matchingResult) {
		QMessageBox::information(nullptr, tr("Information"), tr("Successfully"), tr("Ok"));
	} else {
		QMessageBox::information(nullptr, tr("Information"), tr("Fail"), tr("Ok"));
	}
}
