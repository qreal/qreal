#include "interpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtCore/QDebug>

#include <qrtext/languageToolboxInterface.h>

#include <utils/timelineInterface.h>
#include <utils/tracer.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

using namespace qReal;
using namespace interpreterCore::interpreter;
using namespace interpreterBase::robotModel;

int const maxThreadsCount = 100;

Interpreter::Interpreter(GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, qReal::ProjectManagementInterface const &projectManager
		, BlocksFactoryManagerInterface &blocksFactoryManager
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
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
	connect(
			&mRobotModelManager
			, &interpreterBase::robotModel::RobotModelManagerInterface::allDevicesConfigured
			, this
			, &Interpreter::devicesConfiguredSlot
			);

	connect(
			&mRobotModelManager
			, &interpreterBase::robotModel::RobotModelManagerInterface::connected
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

	if (!mAutoconfigurer.configure(mGraphicalModelApi.children(Id::rootId()), mRobotModelManager.model().name())) {
		return;
	}

	mLanguageToolbox.clear();

	/// @todo Temporarily loading initial configuration from a network of SensorConfigurationProviders.
	///       To be done more adequately.
	for (PortInfo const &port : mRobotModelManager.model().configurablePorts()) {
		QString const modelName = mRobotModelManager.model().name();
		DeviceInfo const deviceInfo = currentConfiguration(modelName, port);
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

void Interpreter::connectedSlot(bool success)
{
	qDebug() << "Interpreter::connectedSlot";

	if (success) {
		if (mRobotModelManager.model().needsConnection()) {
			mInterpretersInterface.errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
		utils::Tracer::debug(utils::Tracer::initialization, "Interpreter::connectedSlot"
				, "Robot connection status: " + QString::number(success));
		mInterpretersInterface.errorReporter()->addError(tr("Can't connect to a robot."));
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

		Id const &currentDiagramId = mInterpretersInterface.activeDiagram();

		details::Thread * const initialThread = new details::Thread(&mGraphicalModelApi
				, mInterpretersInterface, currentDiagramId, *mBlocksTable);

		emit started();

		addThread(initialThread);
	}
}

void Interpreter::threadStopped()
{
	details::Thread * const thread = static_cast<details::Thread *>(sender());

	mThreads.removeAll(thread);
	delete thread;

	if (mThreads.isEmpty()) {
		stopRobot();
	}
}

void Interpreter::newThread(Id const &startBlockId)
{
	details::Thread * const thread = new details::Thread(&mGraphicalModelApi
			, mInterpretersInterface, *mBlocksTable, startBlockId);

	addThread(thread);
}

void Interpreter::addThread(details::Thread * const thread)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopRobot();
	}

	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	connect(thread, &details::Thread::newThread, this, &Interpreter::newThread);

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
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

void Interpreter::reportError(QString const &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
