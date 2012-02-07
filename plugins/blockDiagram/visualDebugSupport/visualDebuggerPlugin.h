#pragma once

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

class VisualDebuggerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	VisualDebuggerPlugin();
	virtual ~VisualDebuggerPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual void activeTabChanged(qReal::Id const & rootElementId);
	
	virtual QPair<QString, PreferencesPage *> preferencesPage();

private slots:
	void debug();
	void debugSingleStep();
	void drawDebuggerStdOutput(QString output);
	void drawDebuggerErrOutput(QString output);
	void generateAndBuild();
	void startDebugger();
	void runProgramWithDebugger();
	void killProgramWithDebugger();
	void closeDebuggerProcessAndThread();
	void placeBreakpointsInDebugger();
	void goToNextBreakpoint();
	void goToNextInstruction();
	///Set program to be debugged
	void configureDebugger();
	void setBreakpointAtStart();
	///Generate, build, start debugger, set program to be debugged,
	///set breakpoint at start and run it in debugger in automatic way
	void startDebugging();
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
	
	QList<qReal::ActionInfo> mActionInfos;
	
	watchListWindow *mWatchListWindow;
	qReal::VisualDebuggerPreferencesPage *mPreferencesPage;
	qReal::BlockParser *mParser;
};

}
}
