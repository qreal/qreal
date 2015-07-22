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

#include <QtWidgets/QApplication>

#include "visualDebuggerPlugin.h"
#include "plugins/toolPluginInterface/systemEvents.h"

using namespace qReal;
using namespace visualDebugger;
using namespace utils;

QString const blockDiagram = "BlockDiagram";

VisualDebuggerPlugin::VisualDebuggerPlugin()
		: mVisualDebugger(nullptr)
		, mDebuggerConnector(nullptr)
		, mErrorReporter(nullptr)
		, mWatchListWindow(nullptr)
		, mParser(nullptr)
		, mPreferencesPage(new VisualDebuggerPreferencesPage())
{
}

VisualDebuggerPlugin::~VisualDebuggerPlugin()
{
}

void VisualDebuggerPlugin::init(PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mParser = new BlockParser(mErrorReporter);

	mVisualDebugger = new VisualDebugger(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, mParser
	);

	mDebuggerConnector = new DebuggerConnector(this);

	connect(&configurator.systemEvents(), &SystemEvents::activeTabChanged
			, this, &VisualDebuggerPlugin::activeTabChanged);
}

QPair<QString, gui::PreferencesPage *> VisualDebuggerPlugin::preferencesPage()
{
	return qMakePair(tr("Block Diagram Debug"), static_cast<gui::PreferencesPage*>(mPreferencesPage));
}

QList<qReal::ActionInfo> VisualDebuggerPlugin::actions()
{
	mVisualDebugMenu = new QMenu(tr("Visual debug"));
	ActionInfo visualDebugMenuInfo(mVisualDebugMenu, "tools");

	mDebugAction = new QAction(tr("Interpret (automatic)"), nullptr);
	mDebugAction->setShortcut(QKeySequence(Qt::Key_F5));
	connect(mDebugAction, SIGNAL(triggered()), this, SLOT(debug()));
	mVisualDebugMenu->addAction(mDebugAction);

	mDebugSingleStepAction = new QAction(tr("Interpret (one step)"), nullptr);
	mDebugSingleStepAction->setShortcut(QKeySequence(Qt::Key_F6));
	connect(mDebugSingleStepAction, SIGNAL(triggered()), this, SLOT(debugSingleStep()));
	mVisualDebugMenu->addAction(mDebugSingleStepAction);

	mWatchListAction = new QAction(tr("Show watch list"), nullptr);
	mWatchListAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	connect(mWatchListAction, SIGNAL(triggered()), this, SLOT(showWatchList()));
	mVisualDebugMenu->addAction(mWatchListAction);

	mVisualDebugWithGdbMenu = new QMenu(tr("Visual debug (with gdb)"));
	ActionInfo visualDebugWithGdbMenuInfo(mVisualDebugWithGdbMenu, "tools");

	mGenerateAndBuildAction = new QAction(tr("Generate and build"), nullptr);
	mGenerateAndBuildAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	connect(mGenerateAndBuildAction, SIGNAL(triggered()), this, SLOT(generateAndBuild()));
	mVisualDebugWithGdbMenu->addAction(mGenerateAndBuildAction);

	mStartDebuggerAction = new QAction(tr("Start debugger (gdb)"), nullptr);
	mStartDebuggerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F6));
	connect(mStartDebuggerAction, SIGNAL(triggered()), this, SLOT(startDebugger()));
	mVisualDebugWithGdbMenu->addAction(mStartDebuggerAction);

	mConfigureAction = new QAction(tr("Configure"), nullptr);
	mConfigureAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F6));
	connect(mConfigureAction, SIGNAL(triggered()), this, SLOT(configureDebugger()));
	mVisualDebugWithGdbMenu->addAction(mConfigureAction);

	mBreakMainAction = new QAction(tr("Set breakpoint at start"), nullptr);
	mBreakMainAction->setShortcut(QKeySequence(Qt::Key_F9));
	connect(mBreakMainAction, SIGNAL(triggered()), this, SLOT(setBreakpointAtStart()));
	mVisualDebugWithGdbMenu->addAction(mBreakMainAction);

	mSetBreakpointsAction = new QAction(tr("Set breakpoints on each element"), nullptr);
	mSetBreakpointsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F9));
	connect(mSetBreakpointsAction, SIGNAL(triggered()), this, SLOT(placeBreakpointsInDebugger()));
	mVisualDebugWithGdbMenu->addAction(mSetBreakpointsAction);

	mRunAction = new QAction(tr("run"), nullptr);
	mRunAction->setShortcut(QKeySequence(Qt::Key_F8));
	connect(mRunAction, SIGNAL(triggered()), this, SLOT(runProgramWithDebugger()));
	mVisualDebugWithGdbMenu->addAction(mRunAction);

	mNextAction = new QAction(tr("next"), nullptr);
	mNextAction->setShortcut(QKeySequence(Qt::Key_F10));
	connect(mNextAction, SIGNAL(triggered()), this, SLOT(goToNextInstruction()));
	mVisualDebugWithGdbMenu->addAction(mNextAction);

	mContAction = new QAction(tr("cont"), nullptr);
	mContAction->setShortcut(QKeySequence(Qt::Key_F11));
	connect(mContAction, SIGNAL(triggered()), this, SLOT(goToNextBreakpoint()));
	mVisualDebugWithGdbMenu->addAction(mContAction);

	mKillAction = new QAction(tr("kill"), nullptr);
	mKillAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
	connect(mKillAction, SIGNAL(triggered()), this, SLOT(killProgramWithDebugger()));
	mVisualDebugWithGdbMenu->addAction(mKillAction);

	mStartDebuggingAction = new QAction(tr("Start debug (automatic)"), nullptr);
	mStartDebuggingAction->setShortcut(QKeySequence(Qt::Key_F7));
	connect(mStartDebuggingAction, SIGNAL(triggered()), this, SLOT(startDebugging()));
	mVisualDebugWithGdbMenu->addAction(mStartDebuggingAction);

	mCloseAllAction = new QAction(tr("Cancel debug"), nullptr);
	mCloseAllAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F12));
	connect(mCloseAllAction, SIGNAL(triggered()), this, SLOT(closeDebuggerProcessAndThread()));
	mVisualDebugWithGdbMenu->addAction(mCloseAllAction);

	connect(mDebuggerConnector, SIGNAL(readyReadStdOutput(QString)), this, SLOT(drawDebuggerStdOutput(QString)));
	connect(mDebuggerConnector, SIGNAL(readyReadErrOutput(QString)), this, SLOT(drawDebuggerErrOutput(QString)));

	mActionInfos << visualDebugMenuInfo << visualDebugWithGdbMenuInfo;

	return mActionInfos;
}

void VisualDebuggerPlugin::activeTabChanged(TabInfo const &info)
{
	bool const enabled = info.rootDiagramId().diagram() == blockDiagram;
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
	if (mWatchListWindow) {
		mWatchListWindow->close();
	}

	mWatchListWindow = new WatchListWindow(mParser);
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
				mErrorReporter->addInformation(tr("Code generated and builded successfully"));
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
