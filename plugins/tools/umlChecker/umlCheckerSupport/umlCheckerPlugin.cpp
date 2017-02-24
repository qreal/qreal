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
	mUmlCheckerRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/examples", true);

	mUmlCheckerFinder = new UmlCheckerFinder(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, mUmlCheckerRepoApi);


	mMetamodelGeneratorSupport = new MetamodelGeneratorSupport(
			configurator.mainWindowInterpretersInterface().errorReporter()
			, &configurator.mainWindowInterpretersInterface());
	mRepoControlIFace = &configurator.repoControlInterface();
	mMainWindowIFace = &configurator.mainWindowInterpretersInterface();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation", "").toString();
	mQRealSourceFilesPath = SettingsManager::value("qrealSourcesLocation").toString();
	mPathToExamples = mQRealSourceFilesPath + "/plugins/umlChecker/examples/";

}

QPair<QString, gui::PreferencesPage *> UmlCheckerPlugin::preferencesPage()
{
	return qMakePair(tr(""), nullptr);
}

QList<qReal::ActionInfo> UmlCheckerPlugin::actions()
{
	mUmlCheckerMenu = new QMenu(tr("UmlChecker"));
	ActionInfo umlCheckerMenuInfo(mUmlCheckerMenu, "tools");

	mSaveAction = new QAction(tr("Parse Solution"), nullptr);
	connect(mSaveAction, SIGNAL(triggered()), this, SLOT(parseSolution()));
	mUmlCheckerMenu->addAction(mSaveAction);

	mActionInfos << umlCheckerMenuInfo;

	return mActionInfos;
}

void UmlCheckerPlugin::parseSolution()
{
	QString const umlSolutionsPath = mPathToExamples + "check.qrs";
	mUmlCheckerRepoApi->open(umlSolutionsPath);
	auto debug = mUmlCheckerFinder->matches();
	QMessageBox::information(nullptr, tr("Information"), tr("successfully"), tr("Ok"));
}
