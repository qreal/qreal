/* Copyright 2012-2016 CyberTech Labs Ltd.
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

#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrutils/interpreter/thread.h>
#include <qrtext/languageToolboxInterface.h>

#include <kitBase/interpreterInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/devicesConfigurationProvider.h>

#include "interpreterCore/interpreter/details/blocksTable.h"
#include "interpreterCore/interpreter/details/sensorVariablesUpdater.h"
#include "interpreterCore/interpreter/details/autoconfigurer.h"


namespace interpreterCore {
namespace interpreter {

/// Interprets robot diagram by executing blocks and sending commands to robot model. Manages models, connection,
/// threads, parser, can automatically configure robot by used blocks on diagram. It is the main class for
/// all interpretation subsystem.
class BlockInterpreter : public kitBase::InterpreterInterface, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor,
	/// @param graphicalModelApi - reference to graphical model, main source of information about interpreted diagram.
	/// @param logicalModelApi - reference to logical model, which contains properties of blocks.
	/// @param interpretersInterface - reference to QReal main window. Used to highlight executing blocks and so on.
	/// @param projectManager - class that manages opening/saving diagrams, needed here to receive notifications
	///        about opening a new save file.
	/// @param blocksFactoryManager - blocks factory, needed to create "code behind" objects for blocks on diagram.
	/// @param robotModelManager - reference to currently selected robot model.
	/// @param parser - parser that is used to analyze and evaluate textual expressions inside properties of blocks.
	/// @param connectToRobotAction - reference to action that connects to robot, interpreter manages its state
	///        depending on success or failure of its own connection attempts.
	BlockInterpreter(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const qReal::ProjectManagementInterface &projectManager
			, BlocksFactoryManagerInterface &blocksFactoryManager
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &languageToolbox
			);

	~BlockInterpreter() override;

	bool isRunning() const;

public slots:
	void connectToRobot() override;
	void interpret() override;
	void stopRobot(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::userStop) override;
	int timeElapsed() const override;
	qReal::IdList supportedDiagrams() const override;

private slots:
	void threadStopped(qReal::interpretation::StopReason reason);
	void newThread(const qReal::Id &startBlockId, const QString &threadId);
	void killThread(const QString &threadId);
	void sendMessage(const QString &threadId, const QString &message);

	void connectedSlot(bool success, const QString &errorString);
	void devicesConfiguredSlot();

private:
	enum InterpreterState {
		interpreting
		, waitingForDevicesConfiguredToLaunch
		, idle
	};

	void addThread(qReal::interpretation::Thread * const thread, const QString &threadId);

	void reportError(const QString &message);

	const qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;

	InterpreterState mState;
	quint64 mInterpretationStartedTimestamp;
	QHash<QString, qReal::interpretation::Thread *> mThreads;  // Has ownership
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	details::BlocksTable *mBlocksTable;  // Has ownership

	details::SensorVariablesUpdater mSensorVariablesUpdater;
	details::Autoconfigurer mAutoconfigurer;

	/// Reference to a parser to be able to clear parser state when starting interpretation.
	qrtext::LanguageToolboxInterface &mLanguageToolbox;
};

}
}
