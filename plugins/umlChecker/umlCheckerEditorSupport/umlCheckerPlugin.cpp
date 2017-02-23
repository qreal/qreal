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
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

using namespace umlChecker;

UmlCheckerPlugin::UmlCheckerPlugin()
	: mRepo(nullptr)
{
}

UmlCheckerPlugin::~UmlCheckerPlugin()
{
}

QList<qReal::ActionInfo> UmlCheckerPlugin::actions()
{
	mCheckerToolMenu = new QMenu(tr("CheckerTool"));
	qReal::ActionInfo checkerToolMenuInfo(mCheckerToolMenu, "tools");

	mAction = new QAction(tr("check"), nullptr);
	connect(mAction, SIGNAL(triggered()), this, SLOT(check()));
	mCheckerToolMenu->addAction(mAction);

	mActionInfos << checkerToolMenuInfo;

	return mActionInfos;
}

void UmlCheckerPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();




//	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
//	mLogicalModelApi = &configurator.logicalModelApi();
//	mGraphicalModelApi = &configurator.graphicalModelApi();

//	mRefactoringWindow = new RefactoringWindow(mMainWindowIFace->windowWidget());
//	connect(mRefactoringWindow, SIGNAL(rejected()), this, SLOT(discardRefactoring()));


//	connect(mRefactoringWindow, SIGNAL(findButtonClicked(QString)), this, SLOT(findRefactoring(QString)));
//	connect(mRefactoringWindow, SIGNAL(findNextButtonClicked()), this, SLOT(findNextRefactoring()));
//	connect(mRefactoringWindow, SIGNAL(discardButtonClicked()), this, SLOT(discardRefactoring()));
//	connect(mRefactoringWindow, SIGNAL(applyButtonClicked()), this, SLOT(applyRefactoring()));




}

void UmlCheckerPlugin::check()
{
	if (QMessageBox::question(mMainWindowInterpretersInterface->windowWidget()
			, tr("checking..")
			, QString(tr("Do you want check your work?"))
			, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
	{
		return;
	}

	QMessageBox::information(nullptr, tr("Information"), tr("Successfully"), tr("Ok"));
}

void UmlCheckerPlugin::chooseNewTask()
{
}

void UmlCheckerPlugin::saveSolution()
{
}

void UmlCheckerPlugin::verifyCurrentSolution()
{
}

void UmlCheckerPlugin::createNewSetOfTemplates()
{
}

void UmlCheckerPlugin::addNewTemplateToCurrentSet()
{
}

void UmlCheckerPlugin::removeTemplateOfCurrentSet()
{
}

void UmlCheckerPlugin::changeTemplatesOfCurrentSet()
{
}

void UmlCheckerPlugin::changeExistingSetTemplates()
{
}

void UmlCheckerPlugin::distinguishBlock()
{
}

