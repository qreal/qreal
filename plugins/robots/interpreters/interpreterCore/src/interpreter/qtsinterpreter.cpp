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

#include "interpreterCore/interpreter/qtsinterpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>

#include <qrtext/languageToolboxInterface.h>

#include <utils/timelineInterface.h>
#include <kitBase/robotModel/robotModelInterface.h>

using namespace qReal;
using namespace interpreterCore::interpreter;
using namespace kitBase::robotModel;

const Id startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");

QtsInterpreter::QtsInterpreter(const GraphicalModelAssistInterface &graphicalModelApi
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
	, mBrick(&robotModelManager.model())
	, mScriptExecControl()
	, mScriptEngine(mBrick, nullptr, nullptr, mScriptExecControl)
{
	// Other components may want to subscribe to allDevicesConfigured() signal because
	// it seems to be the only way to perform robot devices additional initialization.
	// We must let them work out before interpretation starts, so creating queued connection.
	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::allDevicesConfigured
			, this
			, &QtsInterpreter::devicesConfiguredSlot
			, Qt::QueuedConnection
			);

	connect(
			&mRobotModelManager
			, &kitBase::robotModel::RobotModelManagerInterface::connected
			, this
			, &QtsInterpreter::connectedSlot
			);

	connect(&projectManager, &qReal::ProjectManagementInterface::beforeOpen, this, &QtsInterpreter::userStopRobot);

	connectDevicesConfigurationProvider(&mAutoconfigurer);
}

QtsInterpreter::~QtsInterpreter()
{
	delete mBlocksTable;
}

void QtsInterpreter::interpret()
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
		mState = idle;
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

void QtsInterpreter::stopRobot(qReal::interpretation::StopReason reason)
{
	mSensorVariablesUpdater.suspend();
	mRobotModelManager.model().stopRobot();
	mState = idle;
	mBlocksTable->setFailure();
	emit stopped(reason);
}

int QtsInterpreter::timeElapsed() const
{
	return mState == interpreting
			? mRobotModelManager.model().timeline().timestamp() - mInterpretationStartedTimestamp
			: 0;
}

void QtsInterpreter::connectedSlot(bool success, const QString &errorString)
{
	if (success) {
		if (mRobotModelManager.model().needsConnection()) {
			mInterpretersInterface.errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
		if (errorString.isEmpty()) {
			mInterpretersInterface.errorReporter()->addError(tr("Can't connect to a robot."));
		} else {
			mInterpretersInterface.errorReporter()->addError(errorString);
		}
	}

	mActionConnectToRobot.setChecked(success);
}

void QtsInterpreter::devicesConfiguredSlot()
{
	if (mRobotModelManager.model().connectionState() != RobotModelInterface::connectedState) {
		mInterpretersInterface.errorReporter()->addInformation(tr("No connection to robot"));
		mState = idle;
		return;
	}

	if (mState == waitingForDevicesConfiguredToLaunch) {
		mState = interpreting;
		mInterpretationStartedTimestamp = mRobotModelManager.model().timeline().timestamp();

		mSensorVariablesUpdater.run();

		const Id &currentDiagramId = mInterpretersInterface.activeDiagram();

		mScriptEngine.runDirect(QStringLiteral("brick.smile();"), 0);

		emit started();
	}
}

void QtsInterpreter::connectToRobot()
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

void QtsInterpreter::reportError(const QString &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
