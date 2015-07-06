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

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrutils/expressionsParser/expressionsParser.h>

#include "visualDebuggerPreferencesPage.h"

#include "interpreter/visualDebugger.h"
#include "interpreter/debuggerConnector.h"

#include <qrutils/watchListWindow.h>
#include <qrgui/mainWindow/errorReporter.h>

#include <qrkernel/ids.h>

namespace qReal {
namespace visualDebugger {

/// Visual debugger plugin for block diagrams. Allows interpret block diagrams
/// and generate some code and debug it with gdb with highlighting elements on scene
class VisualDebuggerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.visualDebugger.VisualDebuggerPlugin")

public:
	VisualDebuggerPlugin();
	virtual ~VisualDebuggerPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();

private slots:

	/// Start interpretation of block diagram in automatic mode
	void debug();

	/// Make one step of interpretation
	void debugSingleStep();

	/// Draws debugger (gdb) standart output
	void drawDebuggerStdOutput(QString const &output);

	/// Draws debugger (gdb) error output
	void drawDebuggerErrOutput(QString const &output);

	/// Generate code from block diagram and build it in executable file
	void generateAndBuild();

	/// Start debugger process (gdb)
	void startDebugger();

	/// Set executable file in debugger
	void runProgramWithDebugger();

	/// Kill program within debugger
	void killProgramWithDebugger();

	/// Terminate debugger process and close its thread
	void closeDebuggerProcessAndThread();

	/// Place breakpoints in debugger at every element of block diagram
	void placeBreakpointsInDebugger();

	/// Equivalent of 'gdb cont'
	void goToNextBreakpoint();

	/// Equivalent of 'gdb next'
	void goToNextInstruction();

	/// Set program to be debugged
	void configureDebugger();

	/// Set breakpoint at main function in generated code (equivalent of 'break main')
	void setBreakpointAtStart();

	/// Generate, build, start debugger, set program to be debugged,
	/// set breakpoint at start and run it in debugger in automatic way
	void startDebugging();

	/// Show watch list with all declared variables and its values. List updates
	/// dynamically
	void showWatchList();

	void activeTabChanged(qReal::TabInfo const &info);

private:
	qReal::VisualDebugger *mVisualDebugger;
	qReal::DebuggerConnector *mDebuggerConnector;
	qReal::ErrorReporterInterface *mErrorReporter;

	QAction *mDebugAction;
	QAction *mDebugSingleStepAction;
	QAction *mGenerateAndBuildAction;
	QAction *mStartDebuggerAction;
	QAction *mRunAction;
	QAction *mKillAction;
	QAction *mCloseAllAction;
	QAction *mContAction;
	QAction *mNextAction;
	QAction *mSetBreakpointsAction;
	QAction *mConfigureAction;
	QAction *mBreakMainAction;
	QAction *mStartDebuggingAction;
	QAction *mWatchListAction;

	QMenu *mVisualDebugMenu;
	QMenu *mVisualDebugWithGdbMenu;

	QList<qReal::ActionInfo> mActionInfos;

	utils::WatchListWindow *mWatchListWindow;
	qReal::BlockParser *mParser;
	qReal::VisualDebuggerPreferencesPage *mPreferencesPage;
};

}
}
