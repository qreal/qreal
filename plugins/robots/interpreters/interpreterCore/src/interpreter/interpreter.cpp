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

#include "interpreterCore/interpreter/interpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtCore/QDebug>

#include <qrtext/languageToolboxInterface.h>

#include <utils/timelineInterface.h>
#include <utils/tracer.h>
#include <kitBase/robotModel/robotModelInterface.h>

using namespace qReal;
using namespace interpreterCore::interpreter;
using namespace kitBase::robotModel;

const Id startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");
const int maxThreadsCount = 100;

Interpreter::Interpreter(const GraphicalModelAssistInterface &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, const qReal::ProjectManagementInterface &projectManager
		, BlocksFactoryManagerInterface &blocksFactoryManager
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &languageToolbox
		, QAction &connectToRobotAction
		)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mState(idle)
	, mRobotModelManager(robotModelManager)
	, mBlocksTable(new details::BlocksTable(blocksFactoryManager, robotModelManager))
	, mActionConnectToRobot(connectToRobotAction)
	, mSensorVariablesUpdater(robotModelManager, languageToolbox)
	, mAutoconfigurer(mGraphicalModelApi, *mBlocksTable, *mInterpretersInterface.errorReporter())
	, mLanguageToolbox(languageToolbox)
{
	// Other components may want to subscribe to allDevicesConfigured() signal because
	// it seems to be the only way to perform robot devices additional initialization.
	// We must let them work out before interpretation starts, so creating queued connection.
	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::allDevicesConfigured
			, this
			, &Interpreter::devicesConfiguredSlot
			, Qt::QueuedConnection
			);

	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::connected
			, this
			, &Interpreter::connectedSlot
			);

	connect(&projectManager, &qReal::ProjectManagementInterface::beforeOpen, this, &Interpreter::stopRobot);

	connectDevicesConfigurationProvider(&mAutoconfigurer);
}

Interpreter::~Interpreter()
{
	qDeleteAll(mThreads);
	delete mBlocksTable;
}

void Interpreter::interpret()
{
	mInterpretersInterface.errorReporter()->clear();

	if (mRobotModelManager.model().connectionState() != RobotModelInterface::connectedState) {
		mInterpretersInterface.errorReporter()->addInformation(tr("No connection to robot"));
		return;
	}

	if (mState != idle) {
		mInterpretersInterface.errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	mRobotModelManager.model().stopRobot();
	mBlocksTable->clear();
	mState = waitingForDevicesConfiguredToLaunch;

	if (!mAutoconfigurer.configure(mGraphicalModelApi.children(Id::rootId()), mRobotModelManager.model().robotId())) {
		return;
	}

	mLanguageToolbox.clear();

	/// @todo Temporarily loading initial configuration from a network of SensorConfigurationProviders.
	///       To be done more adequately.
	const QString modelName = mRobotModelManager.model().robotId();
	for (const PortInfo &port : mRobotModelManager.model().configurablePorts()) {
		const DeviceInfo deviceInfo = currentConfiguration(modelName, port);
		mRobotModelManager.model().configureDevice(port, deviceInfo);
	}

	mRobotModelManager.model().applyConfiguration();
}

void Interpreter::stopRobot()
{
	mSensorVariablesUpdater.suspend();
	mRobotModelManager.model().stopRobot();
	mState = idle;
	qDeleteAll(mThreads);
	mThreads.clear();
	mBlocksTable->setFailure();
	emit stopped();
}

int Interpreter::timeElapsed() const
{
	return mState == interpreting
			? mRobotModelManager.model().timeline().timestamp() - mInterpretationStartedTimestamp
			: 0;
}

void Interpreter::connectedSlot(bool success, const QString &errorString)
{
	if (success) {
		if (mRobotModelManager.model().needsConnection()) {
			mInterpretersInterface.errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
		utils::Tracer::debug(utils::Tracer::initialization, "Interpreter::connectedSlot"
				, "Robot connection status: " + QString::number(success));
		if (errorString.isEmpty()) {
			mInterpretersInterface.errorReporter()->addError(tr("Can't connect to a robot."));
		} else {
			mInterpretersInterface.errorReporter()->addError(errorString);
		}
	}

	mActionConnectToRobot.setChecked(success);
}

void Interpreter::devicesConfiguredSlot()
{
	utils::Tracer::debug(utils::Tracer::initialization
			, "Interpreter::devicesConfiguredSlot", "Sensors are configured");

	if (mRobotModelManager.model().connectionState() != RobotModelInterface::connectedState) {
		mInterpretersInterface.errorReporter()->addInformation(tr("No connection to robot"));
		mState = idle;
		return;
	}

	if (mState == waitingForDevicesConfiguredToLaunch) {
		mState = interpreting;
		mInterpretationStartedTimestamp = mRobotModelManager.model().timeline().timestamp();

		mSensorVariablesUpdater.run();

		utils::Tracer::debug(utils::Tracer::initialization
				, "Interpreter::devicesConfiguredSlot", "Starting interpretation");

		const Id &currentDiagramId = mInterpretersInterface.activeDiagram();

		qReal::interpretation::Thread * const initialThread = new qReal::interpretation::Thread(&mGraphicalModelApi
				, mInterpretersInterface, startingElementType, currentDiagramId, *mBlocksTable, "main");

		emit started();

		addThread(initialThread, "main");
	}
}

void Interpreter::threadStopped()
{
	qReal::interpretation::Thread * const thread = static_cast<qReal::interpretation::Thread *>(sender());

	mThreads.remove(thread->id());
	delete thread;

	if (mThreads.isEmpty()) {
		stopRobot();
	}
}

void Interpreter::newThread(const Id &startBlockId, const QString &threadId)
{
	if (mThreads.contains(threadId)) {
		reportError(tr("Cannot create new thread with already occupied id %1").arg(threadId));
		stopRobot();
		return;
	}

	qReal::interpretation::Thread * const thread = new qReal::interpretation::Thread(&mGraphicalModelApi
			, mInterpretersInterface, startingElementType, *mBlocksTable, startBlockId, threadId);

	addThread(thread, threadId);
}

void Interpreter::addThread(qReal::interpretation::Thread * const thread, const QString &threadId)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopRobot();
	}

	mThreads[threadId] = thread;
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	connect(thread, &qReal::interpretation::Thread::newThread, this, &Interpreter::newThread);
	connect(thread, &qReal::interpretation::Thread::killThread, this, &Interpreter::killThread);
	connect(thread, &qReal::interpretation::Thread::sendMessage, this, &Interpreter::sendMessage);

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
	}
}

void Interpreter::killThread(const QString &threadId)
{
	if (mThreads.contains(threadId)) {
		mThreads[threadId]->stop();
	} else {
		reportError(tr("Killing non-existent thread %1").arg(threadId));
	}
}

void Interpreter::sendMessage(const QString &threadId, const QString &message)
{
	if (mThreads.contains(threadId)) {
		mThreads[threadId]->newMessage(message);
	}
}

void Interpreter::connectToRobot()
{
	if (mState == interpreting) {
		return;
	}

	if (mRobotModelManager.model().connectionState() == RobotModelInterface::connectedState) {
		mRobotModelManager.model().stopRobot();
		mRobotModelManager.model().disconnectFromRobot();
	} else {
		mRobotModelManager.model().connectToRobot();
	}

	mActionConnectToRobot.setChecked(
			mRobotModelManager.model().connectionState() == RobotModelInterface::connectedState);
}

void Interpreter::disconnectSlot()
{
	mActionConnectToRobot.setChecked(false);
	stopRobot();
}

void Interpreter::reportError(const QString &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
