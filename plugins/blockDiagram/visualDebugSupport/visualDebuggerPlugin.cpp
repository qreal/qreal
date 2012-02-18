#include "visualDebuggerPlugin.h"

Q_EXPORT_PLUGIN2(visualDebugger, qReal::visualDebugger::VisualDebuggerPlugin)

using namespace qReal;
using namespace visualDebugger;
using namespace utils;

const QString blockDiagram = "BlockDiagram";

VisualDebuggerPlugin::VisualDebuggerPlugin()
{
	mPreferencesPage = new VisualDebuggerPreferencesPage();
}

VisualDebuggerPlugin::~VisualDebuggerPlugin()
{
}

void VisualDebuggerPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mParser = new BlockParser(mErrorReporter);

	mVisualDebugger = new VisualDebugger(configurator.logicalModelApi(),
										 configurator.graphicalModelApi(),
										 configurator.mainWindowInterpretersInterface(),
										 mParser);
	mDebuggerConnector = new DebuggerConnector(this);
	mWatchListWindow = NULL;
}

QPair<QString, PreferencesPage *> VisualDebuggerPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Block Diagram Debug"), static_cast<PreferencesPage*>(mPreferencesPage));
}

QList<qReal::ActionInfo> VisualDebuggerPlugin::actions()
{
	mDebugAction = new QAction(QObject::tr("Interpret (automatic)"), NULL);
	ActionInfo debugActionInfo(mDebugAction, "visual debug", "tools");
	connect(mDebugAction, SIGNAL(triggered()), this, SLOT(debug()));
	
	mDebugSingleStepAction = new QAction(QObject::tr("Interpret (one step)"), NULL);
	ActionInfo debugSingleStepActionInfo(mDebugSingleStepAction, "visual debug", "tools");
	connect(mDebugSingleStepAction, SIGNAL(triggered()), this, SLOT(debugSingleStep()));
	
	mWatchListAction = new QAction(QObject::tr("Show watch list"), NULL);
	ActionInfo watchListActionInfo(mWatchListAction, "visual debug", "tools");
	QObject::connect(mWatchListAction, SIGNAL(triggered()), this, SLOT(showWatchList()));
	
	
	mGenerateAndBuildAction = new QAction(QObject::tr("Generate and build"), NULL);
	ActionInfo generateAndBuildActionInfo(mGenerateAndBuildAction, "visual debug (with gdb)", "tools");
	connect(mGenerateAndBuildAction, SIGNAL(triggered()), this, SLOT(generateAndBuild()));
	
	mStartDebuggerAction = new QAction(QObject::tr("Start debugger (gdb)"), NULL);
	ActionInfo startDebuggerActionInfo(mStartDebuggerAction, "visual debug (with gdb)", "tools");
	connect(mStartDebuggerAction, SIGNAL(triggered()), this, SLOT(startDebugger()));
	
	mRunAction = new QAction(QObject::tr("run"), NULL);
	ActionInfo runActionInfo(mRunAction, "visual debug (with gdb)", "tools");
	connect(mRunAction, SIGNAL(triggered()), this, SLOT(runProgramWithDebugger()));
	
	mKillAction = new QAction(QObject::tr("kill"), NULL);
	ActionInfo killActionInfo(mKillAction, "visual debug (with gdb)", "tools");
	connect(mKillAction, SIGNAL(triggered()), this, SLOT(killProgramWithDebugger()));
	
	mCloseAllAction = new QAction(QObject::tr("Cancel debug"), NULL);
	ActionInfo closeAllActionInfo(mCloseAllAction, "visual debug (with gdb)", "tools");
	connect(mCloseAllAction, SIGNAL(triggered()), this, SLOT(closeDebuggerProcessAndThread()));
	
	mContAction = new QAction(QObject::tr("cont"), NULL);
	ActionInfo contActionInfo(mContAction, "visual debug (with gdb)", "tools");
	connect(mContAction, SIGNAL(triggered()), this, SLOT(goToNextBreakpoint()));
	
	mNextAction = new QAction(QObject::tr("next"), NULL);
	ActionInfo nextActionInfo(mNextAction, "visual debug (with gdb)", "tools");
	connect(mNextAction, SIGNAL(triggered()), this, SLOT(goToNextInstruction()));
	
	mSetBreakpointsAction = new QAction(QObject::tr("Set breakpoints on each element"), NULL);
	ActionInfo setBreakpointsActionInfo(mSetBreakpointsAction, "visual debug (with gdb)", "tools");
	connect(mSetBreakpointsAction, SIGNAL(triggered()), this, SLOT(placeBreakpointsInDebugger()));
	
	mConfigureAction = new QAction(QObject::tr("Configure"), NULL);
	ActionInfo configureActionInfo(mConfigureAction, "visual debug (with gdb)", "tools");
	connect(mConfigureAction, SIGNAL(triggered()), this, SLOT(configureDebugger()));
	
	mBreakMainAction = new QAction(QObject::tr("Set breakpoint at start"), NULL);
	ActionInfo breakMainActionInfo(mBreakMainAction, "visual debug (with gdb)", "tools");
	connect(mBreakMainAction, SIGNAL(triggered()), this, SLOT(setBreakpointAtStart()));
	
	mStartDebuggingAction = new QAction(QObject::tr("Start debug (automatic)"), NULL);
	ActionInfo startDebuggingActionInfo(mStartDebuggingAction, "visual debug (with gdb)", "tools");
	connect(mStartDebuggingAction, SIGNAL(triggered()), this, SLOT(startDebugging()));
	
	mActionInfos << debugActionInfo << debugSingleStepActionInfo << watchListActionInfo << generateAndBuildActionInfo
				 << startDebuggerActionInfo << configureActionInfo
				 << setBreakpointsActionInfo << breakMainActionInfo
				 << runActionInfo << nextActionInfo << contActionInfo << killActionInfo
				 << startDebuggingActionInfo << closeAllActionInfo;
	
	connect(mDebuggerConnector, SIGNAL(readyReadStdOutput(QString)), this, SLOT(drawDebuggerStdOutput(QString)));
	connect(mDebuggerConnector, SIGNAL(readyReadErrOutput(QString)), this, SLOT(drawDebuggerErrOutput(QString)));
	
	return mActionInfos;
}

void VisualDebuggerPlugin::activeTabChanged(Id const & rootElementId)
{
	bool const enabled = rootElementId.diagram() == blockDiagram;
	foreach (ActionInfo const &actionInfo, mActionInfos) {
		actionInfo.action()->setEnabled(enabled);
	}
}

void VisualDebuggerPlugin::showWatchList() {
	if (mWatchListWindow != NULL) {
		mWatchListWindow->close();
	}
	mWatchListWindow = new watchListWindow(mParser);
	mWatchListWindow->show();
}

void VisualDebuggerPlugin::debug()
{
	mErrorReporter->clear();
	mVisualDebugger->setCurrentDiagram();
	if (mVisualDebugger->canDebug(VisualDebugger::fullDebug)) {
		mVisualDebugger->debug();
	}
}

void VisualDebuggerPlugin::debugSingleStep()
{
	mErrorReporter->clear();
	mVisualDebugger->setCurrentDiagram();
	if (mVisualDebugger->canDebug(VisualDebugger::singleStepDebug)) {
		mVisualDebugger->debugSingleStep();
	}
}

void VisualDebuggerPlugin::generateAndBuild()
{
	mErrorReporter->clear();
	mVisualDebugger->setCurrentDiagram();

	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)) {
		mVisualDebugger->generateCode();

		if (mVisualDebugger->canBuild()) {
			mDebuggerConnector->run();

			mDebuggerConnector->build();

			if (!mDebuggerConnector->hasBuildError()) {
				mErrorReporter->addInformation("Code generated and builded successfully");
			}
		}
	}
}

void VisualDebuggerPlugin::startDebugger()
{
	mErrorReporter->clear();
	
	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)
		&& !mDebuggerConnector->isDebuggerRunning())
	{
		mVisualDebugger->setDebugType(VisualDebugger::debugWithDebugger);
		mDebuggerConnector->run();
		mDebuggerConnector->startDebugger();
	}
}

void VisualDebuggerPlugin::configureDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->configure();
	}
}

void VisualDebuggerPlugin::setBreakpointAtStart()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mVisualDebugger->setCurrentDiagram();

		mVisualDebugger->createIdByLineCorrelation();

		mDebuggerConnector->sendCommand("break main\n");
	}
}

void VisualDebuggerPlugin::runProgramWithDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("run\n");
	}
}

void VisualDebuggerPlugin::killProgramWithDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("kill\n");
	}
}

void VisualDebuggerPlugin::placeBreakpointsInDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mVisualDebugger->setCurrentDiagram();

		mVisualDebugger->createIdByLineCorrelation();
		if (mVisualDebugger->canComputeBreakpoints()) {
			QList<int>* breakpoints = mVisualDebugger->computeBreakpoints();

			for (int i = 0; i < breakpoints->size(); i++) {
				mDebuggerConnector->sendCommand("break " + QString::number(breakpoints->at(i)) + "\n");
			}

			delete breakpoints;
		}
	}
}

void VisualDebuggerPlugin::goToNextBreakpoint()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("cont\n");
	}
}

void VisualDebuggerPlugin::goToNextInstruction()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("next\n");
	}
}

void VisualDebuggerPlugin::closeDebuggerProcessAndThread()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mVisualDebugger->dehighlight();
		mVisualDebugger->setDebugType(VisualDebugger::noDebug);
		mDebuggerConnector->finishProcess();
	}
}

void VisualDebuggerPlugin::startDebugging()
{
	mErrorReporter->clear();
	
	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)) {
		generateAndBuild();
		startDebugger();
		configureDebugger();
		setBreakpointAtStart();
		runProgramWithDebugger();
	}
}

void VisualDebuggerPlugin::drawDebuggerStdOutput(QString output)
{
	mErrorReporter->addInformation(output);

	if ('1' <= output.at(0) && output.at(0) <= '9') {
		int index = output.indexOf("\t");
		Id idToLigth = mVisualDebugger->getIdByLine(output.mid(0,index).toInt());
		mVisualDebugger->highlight(idToLigth);
	} else {
		QString const fileName = SettingsManager::value("codeFileName", "code.c").toString();

		int index = output.indexOf(fileName + ":");
		if (index > -1) {
			index += (fileName.length() + 1);
			int boundaryIndex = index;
			while ('0' <= output.at(boundaryIndex) && output.at(boundaryIndex) <= '9') {
				boundaryIndex++;
			}
			Id const idToLight = mVisualDebugger->getIdByLine(output.mid(index, boundaryIndex - index).toInt());
			mVisualDebugger->highlight(idToLight);
		}
	}
}

void VisualDebuggerPlugin::drawDebuggerErrOutput(QString output)
{
	mVisualDebugger->dehighlight();
	mVisualDebugger->setDebugType(VisualDebugger::noDebug);
	mErrorReporter->addCritical(output);
}
