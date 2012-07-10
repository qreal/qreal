#include "visualDebuggerPlugin.h"

#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(visualDebugger, qReal::visualDebugger::VisualDebuggerPlugin)

using namespace qReal;
using namespace visualDebugger;
using namespace utils;

QString const blockDiagram = "BlockDiagram";

VisualDebuggerPlugin::VisualDebuggerPlugin()
		: mVisualDebugger(NULL),
		mDebuggerConnector(NULL),
		mErrorReporter(NULL),
		mWatchListWindow(NULL),
		mParser(NULL),
		mPreferencesPage(new VisualDebuggerPreferencesPage())
{
	mAppTranslator.load(":/visualDebugSupport_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

VisualDebuggerPlugin::~VisualDebuggerPlugin()
{
}

void VisualDebuggerPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mParser = new BlockParser(mErrorReporter);

	mVisualDebugger = new VisualDebugger(
			configurator.logicalModelApi(),
			configurator.graphicalModelApi(),
			configurator.mainWindowInterpretersInterface(),
			mParser
	);

	mDebuggerConnector = new DebuggerConnector(this);
}

QPair<QString, PreferencesPage *> VisualDebuggerPlugin::preferencesPage()
{
	return qMakePair(tr("Block Diagram Debug"), static_cast<PreferencesPage*>(mPreferencesPage));
}

QList<qReal::ActionInfo> VisualDebuggerPlugin::actions()
{
	mVisualDebugMenu = new QMenu(tr("Visual debug"));
	ActionInfo visualDebugMenuInfo(mVisualDebugMenu, "tools");

	mDebugAction = new QAction(tr("Interpret (automatic)"), NULL);
	connect(mDebugAction, SIGNAL(triggered()), this, SLOT(debug()));
	mVisualDebugMenu->addAction(mDebugAction);

	mDebugSingleStepAction = new QAction(tr("Interpret (one step)"), NULL);
	connect(mDebugSingleStepAction, SIGNAL(triggered()), this, SLOT(debugSingleStep()));
	mVisualDebugMenu->addAction(mDebugSingleStepAction);

	mWatchListAction = new QAction(tr("Show watch list"), NULL);
	connect(mWatchListAction, SIGNAL(triggered()), this, SLOT(showWatchList()));
	mVisualDebugMenu->addAction(mWatchListAction);


	mVisualDebugWithGdbMenu = new QMenu(tr("Visual debug (with gdb)"));
	ActionInfo visualDebugWithGdbMenuInfo(mVisualDebugWithGdbMenu, "tools");

	mGenerateAndBuildAction = new QAction(tr("Generate and build"), NULL);
	connect(mGenerateAndBuildAction, SIGNAL(triggered()), this, SLOT(generateAndBuild()));
	mVisualDebugWithGdbMenu->addAction(mGenerateAndBuildAction);

	mStartDebuggerAction = new QAction(tr("Start debugger (gdb)"), NULL);
	connect(mStartDebuggerAction, SIGNAL(triggered()), this, SLOT(startDebugger()));
	mVisualDebugWithGdbMenu->addAction(mStartDebuggerAction);

	mConfigureAction = new QAction(tr("Configure"), NULL);
	connect(mConfigureAction, SIGNAL(triggered()), this, SLOT(configureDebugger()));
	mVisualDebugWithGdbMenu->addAction(mConfigureAction);

	mSetBreakpointsAction = new QAction(tr("Set breakpoints on each element"), NULL);
	connect(mSetBreakpointsAction, SIGNAL(triggered()), this, SLOT(placeBreakpointsInDebugger()));
	mVisualDebugWithGdbMenu->addAction(mSetBreakpointsAction);

	mBreakMainAction = new QAction(tr("Set breakpoint at start"), NULL);
	connect(mBreakMainAction, SIGNAL(triggered()), this, SLOT(setBreakpointAtStart()));
	mVisualDebugWithGdbMenu->addAction(mBreakMainAction);

	mRunAction = new QAction(tr("run"), NULL);
	connect(mRunAction, SIGNAL(triggered()), this, SLOT(runProgramWithDebugger()));
	mVisualDebugWithGdbMenu->addAction(mRunAction);

	mNextAction = new QAction(tr("next"), NULL);
	connect(mNextAction, SIGNAL(triggered()), this, SLOT(goToNextInstruction()));
	mVisualDebugWithGdbMenu->addAction(mNextAction);

	mContAction = new QAction(tr("cont"), NULL);
	connect(mContAction, SIGNAL(triggered()), this, SLOT(goToNextBreakpoint()));
	mVisualDebugWithGdbMenu->addAction(mContAction);

	mKillAction = new QAction(tr("kill"), NULL);
	connect(mKillAction, SIGNAL(triggered()), this, SLOT(killProgramWithDebugger()));
	mVisualDebugWithGdbMenu->addAction(mKillAction);

	mStartDebuggingAction = new QAction(tr("Start debug (automatic)"), NULL);
	connect(mStartDebuggingAction, SIGNAL(triggered()), this, SLOT(startDebugging()));
	mVisualDebugWithGdbMenu->addAction(mStartDebuggingAction);

	mCloseAllAction = new QAction(tr("Cancel debug"), NULL);
	connect(mCloseAllAction, SIGNAL(triggered()), this, SLOT(closeDebuggerProcessAndThread()));
	mVisualDebugWithGdbMenu->addAction(mCloseAllAction);

	connect(mDebuggerConnector, SIGNAL(readyReadStdOutput(QString)), this, SLOT(drawDebuggerStdOutput(QString)));
	connect(mDebuggerConnector, SIGNAL(readyReadErrOutput(QString)), this, SLOT(drawDebuggerErrOutput(QString)));

	mActionInfos << visualDebugMenuInfo << visualDebugWithGdbMenuInfo;

	return mActionInfos;
}

void VisualDebuggerPlugin::activeTabChanged(Id const &rootElementId)
{
	bool const enabled = rootElementId.diagram() == blockDiagram;
	foreach (ActionInfo const &actionInfo, mActionInfos) {
		if (actionInfo.isAction()) {
			actionInfo.action()->setEnabled(enabled);
		} else {
			actionInfo.menu()->setEnabled(enabled);
		}
	}
}

void VisualDebuggerPlugin::showWatchList()
{
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
				mErrorReporter->addInformation(
						tr("Code generated and builded successfully"));
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

			foreach (int const &breakpoint, *breakpoints) {
				mDebuggerConnector->sendCommand("break " + QString::number(breakpoint) + "\n");
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

void VisualDebuggerPlugin::drawDebuggerStdOutput(QString const &output)
{
	mErrorReporter->addInformation(output);

	if ('1' <= output.at(0) && output.at(0) <= '9') {
		int const index = output.indexOf("\t");
		Id const idToLigth = mVisualDebugger->getIdByLine(output.mid(0,index).toInt());
		mVisualDebugger->highlight(idToLigth);
	} else {
		QString const fileName = SettingsManager::value("codeFileName").toString();

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

void VisualDebuggerPlugin::drawDebuggerErrOutput(QString const &output)
{
	mVisualDebugger->dehighlight();
	mVisualDebugger->setDebugType(VisualDebugger::noDebug);
	mErrorReporter->addCritical(output);
}
