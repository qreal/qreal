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

#include <QtWidgets/QMenu>

#include <qrkernel/ids.h>
#include <qrutils/metamodelGeneratorSupport.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/mainWindow/errorReporter.h>

#include <qrrepo/repoApi.h>

#include "umlCheckerFinder.h"


namespace qReal {
namespace umlChecker {


class UmlCheckerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.umlChecker.UmlCheckerPlugin")

public:
	UmlCheckerPlugin();
	virtual ~UmlCheckerPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();

private slots:

	/// save .qrs file with the refactoring rule on active diagram,
	/// only if this diagram has refactoring type (RefactoringDiagramNode)
	/// and picture (.png) with this rule.
	/// names of .qrs and .png are the same as name on the diagram
	void parseSolution();



private:



	qReal::ErrorReporterInterface *mErrorReporter;

	QMenu *mUmlCheckerMenu;
	QMenu *mPlaceMenu;

	QAction *mSaveAction;


	LogicalModelAssistInterface *mLogicalModelApi;
	GraphicalModelAssistInterface *mGraphicalModelApi;
	qrRepo::RepoControlInterface *mRepoControlIFace;
	gui::MainWindowInterpretersInterface *mMainWindowIFace;

	QString mQRealSourceFilesPath;
	QString mPathToExamples;

	QList<qReal::ActionInfo> mActionInfos;
	QStringList mEditorElementNames;

	qrRepo::RepoApi *mUmlCheckerRepoApi;
	UmlCheckerFinder *mUmlCheckerFinder;

	utils::MetamodelGeneratorSupport *mMetamodelGeneratorSupport;

	QList<QHash<Id, Id> > mMatches;
	QHash <Id, Id> mCurrentMatch;
	IdList mSelectedElementsOnActiveDiagram;
};
}
}
