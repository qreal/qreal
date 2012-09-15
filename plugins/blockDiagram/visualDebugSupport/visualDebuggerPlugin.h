#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "../../../qrutils/expressionsParser/expressionsParser.h"

#include "visualDebuggerPreferencesPage.h"

#include "interpreter/visualDebugger.h"
#include "interpreter/debuggerConnector.h"

#include "../../../qrutils/watchListWindow.h"
#include "../../../qrgui/mainwindow/errorReporter.h"

#include "../../../qrkernel/ids.h"

namespace qReal {
namespace visualDebugger {

/// Visual debugger plugin for block diagrams. Allows interpret block diagrams
/// and generate some code and debug it with gdb with highlighting elements on scene
class VisualDebuggerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	VisualDebuggerPlugin();
	virtual ~VisualDebuggerPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual void activeTabChanged(qReal::Id const &rootElementId);
	
	virtual QPair<QString, PreferencesPage *> preferencesPage();

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
	
	utils::watchListWindow *mWatchListWindow;
	qReal::BlockParser *mParser;
	qReal::VisualDebuggerPreferencesPage *mPreferencesPage;
	
	QTranslator mAppTranslator;
};

}
}
