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
#include <QInputDialog>

#include <qrkernel/ids.h>
#include <qrutils/metamodelGeneratorSupport.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/mainWindow/errorReporter.h>

#include <qrrepo/repoApi.h>

#include "umlCheckerHandler.h"
#include "umlCheckerTmplWindow.h"
#include "umlCheckerPreferencePage.h"
#include "umlCheckerTemplate.h"
#include "umlCheckerPerfectSolution.h"

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
	void parseSolution();
	void addElementsToBlock();
	void addElementsToEdge();
	void savePerfectSolution();
	void openTemplatesWindow(const QString &name);
	void save();
	void saveEdges();

	void saveTemplate();

private:
	qReal::ErrorReporterInterface *mErrorReporter;
	gui::MainWindowInterpretersInterface *mMainWindowIFace;
	qrRepo::RepoControlInterface *mRepoControlIFace;


	QMenu *mUmlCheckerMenu;
	QMenu *mPlaceMenu;

	QAction *mSavePerfectSolution;
	QAction *mParseAction;
	QAction *mAddBlockAction;
	QAction *mAddEdgesAction;
	QAction *mSaveTemplate;

	QAction *mOpenTemplatesWindowAction;

	UmlCheckerTmplWindow *mTemplatesWindow;
	UmlCheckerPreferencePage *mPreferencesPage;

	UmlCheckerTemplate *mUmlCheckerTemplate;
	UmlCheckerPerfectSolution *mUmlCheckerPerfectSolution;

	QString mQRealSourceFilesPath;
	QList<qReal::ActionInfo> mActionInfos;

	qrRepo::RepoApi *mOrdinaryRepoApi;
	qrRepo::RepoApi *mPerfectRepoApi;
	qrRepo::RepoApi *mRepoApi;

	UmlCheckerHandler *mHandler;
};
}
}
