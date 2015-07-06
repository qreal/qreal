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

#include "deploymentInterpreterPlugin.h"

#include <QtWidgets/QMainWindow>

#include "blocksTable.h"

using namespace qReal;
using namespace deployment;

Id const startingElementType = Id("DeploymentEditor", "DeploymentDiagram", "InitialNode");

DeploymentInterpreterPlugin::DeploymentInterpreterPlugin()
	: mBlocksTable(nullptr)
	, mInterpreter(nullptr)
	, mRunAction(new QAction(QIcon(":/images/run_deployment.png"), QObject::tr("Run deployment"), nullptr))
	, mStopAction(new QAction(QIcon(":/images/stop_deployment.png"), QObject::tr("Stop deployment"), nullptr))
{
	mRunAction->setShortcut(QKeySequence(Qt::Key_F5));
	mStopAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
}

DeploymentInterpreterPlugin::~DeploymentInterpreterPlugin()
{
	delete mBlocksTable;
	delete mInterpreter;
}

void DeploymentInterpreterPlugin::init(PluginConfigurator const &configurator)
{
	ShellWidget * const shell = new ShellWidget(configurator.mainWindowInterpretersInterface().windowWidget());
	placeShellWidget(shell, configurator.mainWindowInterpretersInterface().windowWidget());

	mBlocksTable = new BlocksTable(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, *configurator.mainWindowInterpretersInterface().errorReporter()
			, mTextLanguage
			, shell);
	mInterpreter = new qReal::interpretation::Interpreter(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, *mBlocksTable
			, mTextLanguage
			, startingElementType);
	connect(&configurator.projectManager(), &qReal::ProjectManagementInterface::beforeOpen
			, mInterpreter, &interpretation::Interpreter::stopInterpretation);
	connect(mRunAction, &QAction::triggered, mInterpreter, &interpretation::Interpreter::startInterpretation);
	connect(mStopAction, &QAction::triggered, mInterpreter, &interpretation::Interpreter::stopInterpretation);

	connect(&configurator.systemEvents(), &SystemEvents::activeTabChanged, [=](TabInfo const &info) {
		const bool isOurTab = info.rootDiagramId().editor() == startingElementType.editor()
				&& info.rootDiagramId().diagram() == startingElementType.diagram();
		mRunAction->setVisible(isOurTab);
		mStopAction->setVisible(isOurTab);
		mShellDock->setVisible(isOurTab);
	});
}

QList<ActionInfo> DeploymentInterpreterPlugin::actions()
{
	return { qReal::ActionInfo(mRunAction, "interpreters", "tools")
			, qReal::ActionInfo(mStopAction, "interpreters", "tools")
	};
}

QList<HotKeyActionInfo> DeploymentInterpreterPlugin::hotKeyActions()
{
	return { qReal::HotKeyActionInfo("DeploymentInterpreter.Run", QObject::tr("Run deployment"), mRunAction)
		, qReal::HotKeyActionInfo("DeploymentInterpreter.Stop", QObject::tr("Stop deployment"), mStopAction)
	};
}

void DeploymentInterpreterPlugin::placeShellWidget(QWidget *shell, QWidget *mainWindowWidget)
{
	/// @todo: This is pretty hacky implementation. Rewrite this using mainWindowDockInterface
	QMainWindow * const mainWindow = dynamic_cast<QMainWindow *>(mainWindowWidget);
	QDockWidget * const shellDock = new QDockWidget(tr("Console output"), mainWindow);
	shellDock->setWidget(shell);
	mainWindow->tabifyDockWidget(mainWindow->findChild<QDockWidget *>("errorDock"), shellDock);
	mShellDock = shellDock;
}
