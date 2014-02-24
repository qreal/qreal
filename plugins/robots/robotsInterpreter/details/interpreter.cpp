#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtWidgets/QAction>

#include "interpreter.h"

#include "details/autoconfigurer.h"
#include "details/robotImplementations/unrealRobotModelImplementation.h"
#include "details/robotCommunication/bluetoothRobotCommunicationThread.h"
#include "details/robotCommunication/usbRobotCommunicationThread.h"
#include "details/robotCommunication/tcpRobotCommunicationThread.h"
#include "details/tracer.h"
#include "details/debugHelper.h"

#include "sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

int const maxThreadsCount = 100;

Interpreter::Interpreter()
	: SensorsConfigurationProvider("Interpreter")
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mInterpretersInterface(nullptr)
	, mState(idle)
	, mRobotModel(new RobotModel())
	, mBlocksTable(nullptr)
	, mParser(nullptr)
	, mRobotCommunication(new RobotCommunicator())
	, mImplementationType(robots::enums::robotModelType::null)
	, mWatchListWindow(nullptr)
	, mActionConnectToRobot(nullptr)
{
	mD2RobotModel = new d2Model::D2RobotModel();
	mD2ModelWidget = mD2RobotModel->createModelWidget();
	mD2ModelWidget->connectSensorsConfigurationProvider(this);

	connect(mD2ModelWidget, SIGNAL(modelChanged(QDomDocument)), this, SLOT(on2dModelChanged(QDomDocument)));
	connect(mRobotModel, SIGNAL(disconnected()), this, SLOT(disconnectSlot()));
	connect(mRobotModel, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
	connect(mRobotModel, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	connect(mD2ModelWidget, SIGNAL(d2WasClosed()), this, SLOT(stopRobot()));
	connect(mRobotCommunication, SIGNAL(errorOccured(QString)), this, SLOT(reportError(QString)));
}

void Interpreter::init(GraphicalModelAssistInterface const &graphicalModelApi
	, LogicalModelAssistInterface &logicalModelApi
	, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
	, qReal::ProjectManagementInterface const &projectManager)
{
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mInterpretersInterface = &interpretersInterface;

	mParser = new RobotsBlockParser(mInterpretersInterface->errorReporter(), [this] () {
		return mState == interpreting ? mRobotModel->timeline()->timestamp() - mInterpretationStartedTimestamp : 0;
	});
	mBlocksTable = new BlocksTable(graphicalModelApi, logicalModelApi, mRobotModel
			, mInterpretersInterface->errorReporter(), mParser);

	connect(&projectManager, SIGNAL(beforeOpen(QString)), this, SLOT(stopRobot()));

	robots::enums::robotModelType::robotModelTypeEnum const modelType
			= static_cast<robots::enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());

	Tracer::debug(
			tracer::enums::initialization
			, "Interpreter::init"
			, "Going to set robot implementation, model type is " + DebugHelper::toString(modelType)
			);

	setRobotImplementation(modelType);

	mWatchListWindow = new utils::WatchListWindow(mParser, mInterpretersInterface->windowWidget());

	mGraphicsWatch = new utils::sensorsGraph::SensorsGraph(mParser, mInterpretersInterface->windowWidget());
}

Interpreter::~Interpreter()
{
	foreach (Thread * const thread, mThreads) {
		delete thread;
	}
	delete mBlocksTable;
}

void Interpreter::interpret()
{
	Tracer::debug(tracer::enums::initialization, "Interpreter::interpret", "Preparing for interpretation");

	mInterpretersInterface->errorReporter()->clear();

	Id const &currentDiagramId = mInterpretersInterface->activeDiagram();

	if (!mConnected) {
		mInterpretersInterface->errorReporter()->addInformation(tr("No connection to robot"));
		return;
	}
	if (mState != idle) {
		mInterpretersInterface->errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	mBlocksTable->clear();
	mState = waitingForSensorsConfiguredToLaunch;
	mBlocksTable->setIdleForBlocks();

	Autoconfigurer configurer(*mGraphicalModelApi, mBlocksTable, mInterpretersInterface->errorReporter(), mRobotModel);
	if (!configurer.configure(currentDiagramId)) {
		return;
	}

	mGraphicsWatch->startJob();
}

void Interpreter::stopRobot()
{
	mTimer.stop();
	mRobotModel->stopRobot();
	mState = idle;
	foreach (Thread * const thread, mThreads) {
		delete thread;
		mThreads.removeAll(thread);
	}
	mBlocksTable->setFailure();

	mGraphicsWatch->stopJob();
}

void Interpreter::showWatchList()
{
	mWatchListWindow->show();

	mGraphicsWatch->show();
}

void Interpreter::onTabChanged(Id const &diagramId, bool enabled)
{
	if (enabled) {
		Id const logicalId = mGraphicalModelApi->logicalId(diagramId);
		QString const xml = mLogicalModelApi->propertyByRoleName(logicalId, "worldModel").toString();
		QDomDocument worldModel;
		worldModel.setContent(xml);
		mD2ModelWidget->loadXml(worldModel);
		loadSensorConfiguration(logicalId);
		enableD2ModelWidgetRunStopButtons();
	} else {
		disableD2ModelWidgetRunStopButtons();
	}
}

void Interpreter::closeWatchList()
{
	if (mWatchListWindow) {
		mWatchListWindow->setVisible(false);
	}

	if (mGraphicsWatch) {
		mGraphicsWatch->setVisible(false);
	}
}

void Interpreter::closeD2ModelWidget()
{
	if (mD2ModelWidget) {
		mD2ModelWidget->close();
	}
}

void Interpreter::showD2ModelWidget(bool isVisible)
{
	mD2ModelWidget->init(isVisible);
	if (isVisible) {
		mD2ModelWidget->activateWindow();
		mD2ModelWidget->show();
	}
}

void Interpreter::setD2ModelWidgetActions(QAction *runAction, QAction *stopAction)
{
	mD2ModelWidget->setD2ModelWidgetActions(runAction, stopAction);
}

void Interpreter::enableD2ModelWidgetRunStopButtons()
{
	mD2ModelWidget->enableRunStopButtons();
}

void Interpreter::disableD2ModelWidgetRunStopButtons()
{
	mD2ModelWidget->disableRunStopButtons();
}

void Interpreter::setRobotImplementation(robots::enums::robotModelType::robotModelTypeEnum implementationType)
{
	mConnected = false;
	mActionConnectToRobot->setChecked(false);
	robotImplementations::AbstractRobotModelImplementation *robotImpl
			= robotImplementations::AbstractRobotModelImplementation::robotModel(implementationType
					, mRobotCommunication, mD2RobotModel);
	setRobotImplementation(robotImpl);
	mImplementationType = implementationType;
	if (mImplementationType != robots::enums::robotModelType::nxt) {
		mRobotModel->init();
	}
}

void Interpreter::connectedSlot(bool success)
{
	if (success) {
		if (mRobotModel->needsConnection()) {
			mInterpretersInterface->errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
		Tracer::debug(tracer::enums::initialization, "Interpreter::connectedSlot", "Robot connection status: " + QString::number(success));
		mInterpretersInterface->errorReporter()->addError(tr("Can't connect to a robot."));
	}
	mConnected = success;
	mActionConnectToRobot->setChecked(success);
}

void Interpreter::sensorsConfiguredSlot()
{
	Tracer::debug(tracer::enums::initialization, "Interpreter::sensorsConfiguredSlot", "Sensors are configured");

	mConnected = true;
	mActionConnectToRobot->setChecked(mConnected);

	resetVariables();

	mRobotModel->nextBlockAfterInitial(mConnected);

	if (mState == waitingForSensorsConfiguredToLaunch) {
		mState = interpreting;
		mInterpretationStartedTimestamp = mRobotModel->timeline()->timestamp();

		runTimer();

		Tracer::debug(tracer::enums::initialization, "Interpreter::sensorsConfiguredSlot", "Starting interpretation");
		mRobotModel->startInterpretation();

		Id const &currentDiagramId = mInterpretersInterface->activeDiagram();
		Thread * const initialThread = new Thread(mGraphicalModelApi
				, *mInterpretersInterface, currentDiagramId, *mBlocksTable);
		addThread(initialThread);
	}
}

void Interpreter::threadStopped()
{
	Thread *thread = static_cast<Thread *>(sender());

	mThreads.removeAll(thread);
	delete thread;

	if (mThreads.isEmpty()) {
		stopRobot();
	}
}

void Interpreter::newThread(details::blocks::Block * const startBlock)
{
	Thread * const thread = new Thread(mGraphicalModelApi
			, *mInterpretersInterface, *mBlocksTable, startBlock->id());
	addThread(thread);
}

void Interpreter::configureSensors(
		robots::enums::sensorType::SensorTypeEnum const &port1
		, robots::enums::sensorType::SensorTypeEnum const &port2
		, robots::enums::sensorType::SensorTypeEnum const &port3
		, robots::enums::sensorType::SensorTypeEnum const &port4)
{
	if (mConnected) {
		mRobotModel->configureSensors(port1, port2, port3, port4);
	}

	updateGraphicWatchSensorsList();
}

void Interpreter::addThread(details::Thread * const thread)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopRobot();
	}

	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));
	connect(thread, SIGNAL(newThread(details::blocks::Block*const)), this, SLOT(newThread(details::blocks::Block*const)));

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
	}
}

interpreters::robots::details::RobotModel *Interpreter::robotModel()
{
	return mRobotModel;
}

void Interpreter::setRobotModel(details::RobotModel * const robotModel)
{
	mRobotModel = robotModel;
}

void Interpreter::setRobotImplementation(details::robotImplementations::AbstractRobotModelImplementation *robotImpl)
{
	mRobotModel->setRobotImplementation(robotImpl);
	if (robotImpl) {
		connect(mRobotModel, SIGNAL(connected(bool)), this, SLOT(runTimer()));
	}
}

void Interpreter::runTimer()
{
	if (mRobotModel->sensor(robots::enums::inputPort::port1)) {
		connect(
				mRobotModel->sensor(robots::enums::inputPort::port1)->sensorImpl()
				, SIGNAL(response(int))
				, this
				, SLOT(responseSlot1(int))
				, Qt::UniqueConnection
				);

		connect(
				mRobotModel->sensor(robots::enums::inputPort::port1)->sensorImpl()
				, SIGNAL(failure())
				, this
				, SLOT(slotFailure())
				, Qt::UniqueConnection
				);
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port2)) {
		connect(
				mRobotModel->sensor(robots::enums::inputPort::port2)->sensorImpl()
				, SIGNAL(response(int))
				, this
				, SLOT(responseSlot2(int))
				, Qt::UniqueConnection
				);

		connect(
				mRobotModel->sensor(robots::enums::inputPort::port2)->sensorImpl()
				, SIGNAL(failure())
				, this
				, SLOT(slotFailure())
				, Qt::UniqueConnection
				);
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port3)) {
		connect(
				mRobotModel->sensor(robots::enums::inputPort::port3)->sensorImpl()
				, SIGNAL(response(int))
				, this
				, SLOT(responseSlot3(int))
				, Qt::UniqueConnection
				);

		connect(
				mRobotModel->sensor(robots::enums::inputPort::port3)->sensorImpl()
				, SIGNAL(failure())
				, this
				, SLOT(slotFailure())
				, Qt::UniqueConnection
				);
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port4)) {
		connect(
				mRobotModel->sensor(robots::enums::inputPort::port4)->sensorImpl()
				, SIGNAL(response(int))
				, this
				, SLOT(responseSlot4(int))
				, Qt::UniqueConnection
				);

		connect(
				mRobotModel->sensor(robots::enums::inputPort::port4)->sensorImpl()
				, SIGNAL(failure())
				, this
				, SLOT(slotFailure())
				, Qt::UniqueConnection
				);
	}

	connect(mRobotModel->encoderA().encoderImpl(), SIGNAL(response(int))
			, this, SLOT(responseSlotA(int)), Qt::UniqueConnection);
	connect(mRobotModel->encoderA().encoderImpl(), SIGNAL(failure())
			, this, SLOT(slotFailure()), Qt::UniqueConnection);
	connect(mRobotModel->encoderB().encoderImpl(), SIGNAL(response(int))
			, this, SLOT(responseSlotB(int)), Qt::UniqueConnection);
	connect(mRobotModel->encoderB().encoderImpl(), SIGNAL(failure())
			, this, SLOT(slotFailure()), Qt::UniqueConnection);
	connect(mRobotModel->encoderC().encoderImpl(), SIGNAL(response(int))
			, this, SLOT(responseSlotC(int)), Qt::UniqueConnection);
	connect(mRobotModel->encoderC().encoderImpl(), SIGNAL(failure())
			, this, SLOT(slotFailure()), Qt::UniqueConnection);

	mRobotModel->nullifySensors();
	if (!mTimer.isActive()) {
		readSensorValues();
		mTimer.start(25);
		connect(&mTimer, SIGNAL(timeout()), this, SLOT(readSensorValues()), Qt::UniqueConnection);
	}
}

void Interpreter::readSensorValues()
{
	if (mState == idle) {
		return;
	}

	if (mRobotModel->sensor(robots::enums::inputPort::port1)) {
		mRobotModel->sensor(robots::enums::inputPort::port1)->read();
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port2)) {
		mRobotModel->sensor(robots::enums::inputPort::port2)->read();
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port3)) {
		mRobotModel->sensor(robots::enums::inputPort::port3)->read();
	}
	if (mRobotModel->sensor(robots::enums::inputPort::port4)) {
		mRobotModel->sensor(robots::enums::inputPort::port4)->read();
	}

	mRobotModel->encoderA().read();
	mRobotModel->encoderB().read();
	mRobotModel->encoderC().read();
}

void Interpreter::slotFailure()
{
	Tracer::debug(tracer::enums::autoupdatedSensorValues, "Interpreter::slotFailure", "");
}

void Interpreter::responseSlot1(int sensorValue)
{
	updateSensorValues("sensor1", sensorValue);
}

void Interpreter::responseSlot2(int sensorValue)
{
	updateSensorValues("sensor2", sensorValue);
}

void Interpreter::responseSlot3(int sensorValue)
{
	updateSensorValues("sensor3", sensorValue);
}

void Interpreter::responseSlot4(int sensorValue)
{
	updateSensorValues("sensor4", sensorValue);
}

void Interpreter::responseSlotA(int encoderValue)
{
	updateSensorValues("encoderA", encoderValue);
}

void Interpreter::responseSlotB(int encoderValue)
{
	updateSensorValues("encoderB", encoderValue);
}

void Interpreter::responseSlotC(int encoderValue)
{
	updateSensorValues("encoderC", encoderValue);
}

void Interpreter::updateSensorValues(QString const &sensorVariableName, int sensorValue)
{
	mParser->variables()[sensorVariableName]->setValue(sensorValue);
	Tracer::debug(
			tracer::enums::autoupdatedSensorValues
			, "Interpreter::updateSensorValues"
			, sensorVariableName + QString::number(sensorValue)
			);
}

void Interpreter::resetVariables()
{
	int const resetValue = 0;
	responseSlot1(resetValue);
	responseSlot2(resetValue);
	responseSlot3(resetValue);
	responseSlot4(resetValue);
}

void Interpreter::connectToRobot()
{
	if (mState == interpreting) {
		return;
	}
	if (mConnected) {
		mRobotModel->stopRobot();
		mRobotModel->disconnectFromRobot();
	} else {
		mRobotModel->init();
		configureSensors(
				  static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt()));
	}
	mActionConnectToRobot->setChecked(mConnected);
}

void Interpreter::disconnectSlot()
{
	mActionConnectToRobot->setChecked(false);
	mConnected = false;
}

void Interpreter::setRobotModelType(robots::enums::robotModelType::robotModelTypeEnum robotModelType)
{
	setRobotImplementation(robotModelType);
}

void Interpreter::setCommunicator(QString const &valueOfCommunication)
{
	if (valueOfCommunication == mLastCommunicationValue) {
		return;
	}

	RobotCommunicationThreadInterface *communicator = NULL;
	if (valueOfCommunication == "bluetooth") {
		communicator = new BluetoothRobotCommunicationThread();
	} else if (valueOfCommunication == "usb") {
		communicator = new UsbRobotCommunicationThread();
	} else {
		communicator = new TcpRobotCommunicationThread();
	}

	mLastCommunicationValue = valueOfCommunication;

	mRobotCommunication->setRobotCommunicationThreadObject(communicator);
}

void Interpreter::setConnectRobotAction(QAction *actionConnect)
{
	mActionConnectToRobot = actionConnect;
}

void Interpreter::reportError(QString const &message)
{
	mInterpretersInterface->errorReporter()->addError(message);
}

void Interpreter::on2dModelChanged(QDomDocument const &xml)
{
	Id const currentDiagramId = mInterpretersInterface->activeDiagram();
	Id const logicalId = mGraphicalModelApi->logicalId(currentDiagramId);
	if (logicalId != Id() && logicalId != Id::rootId()) {
		mLogicalModelApi->setPropertyByRoleName(logicalId, xml.toString(4), "worldModel");
	}
}

void Interpreter::saveSensorConfiguration()
{
	Id const currentDiagramId = mInterpretersInterface->activeDiagram();
	Id const logicalId = mGraphicalModelApi->logicalId(currentDiagramId);
	if (logicalId != Id() && logicalId != Id::rootId()) {
		int const sensor1Value = SettingsManager::value("port1SensorType").toInt();
		int const sensor2Value = SettingsManager::value("port2SensorType").toInt();
		int const sensor3Value = SettingsManager::value("port3SensorType").toInt();
		int const sensor4Value = SettingsManager::value("port4SensorType").toInt();
		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor1Value), "sensor1Value");
		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor2Value), "sensor2Value");
		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor3Value), "sensor3Value");
		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor4Value), "sensor4Value");
	}
}

void Interpreter::loadSensorConfiguration(Id const &diagramId)
{
	int const sensor1Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor1Value").toInt();
	int const sensor2Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor2Value").toInt();
	int const sensor3Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor3Value").toInt();
	int const sensor4Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor4Value").toInt();

	sensorConfigurationChanged(
			robots::enums::inputPort::port1
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(sensor1Value)
			);

	sensorConfigurationChanged(
			robots::enums::inputPort::port2
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(sensor2Value)
			);

	sensorConfigurationChanged(
			robots::enums::inputPort::port3
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(sensor3Value)
			);

	sensorConfigurationChanged(
			robots::enums::inputPort::port4
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(sensor4Value)
			);
}

utils::WatchListWindow *Interpreter::watchWindow() const
{
	return mWatchListWindow;
}

void Interpreter::updateGraphicWatchSensorsList()
{
	mGraphicsWatch->addTrackingObject(0, QString("sensor1")
			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
					(SettingsManager::instance()->value("port1SensorType").toInt())));
	mGraphicsWatch->addTrackingObject(1, QString("sensor2")
			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
					(SettingsManager::instance()->value("port2SensorType").toInt())));
	mGraphicsWatch->addTrackingObject(2, QString("sensor3")
			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
					(SettingsManager::instance()->value("port3SensorType").toInt())));
	mGraphicsWatch->addTrackingObject(3, QString("sensor4")
			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
					(SettingsManager::instance()->value("port4SensorType").toInt())));
}

void Interpreter::onSensorConfigurationChanged(
		robots::enums::inputPort::InputPortEnum port
		, robots::enums::sensorType::SensorTypeEnum type
		)
{
	Q_UNUSED(port);
	Q_UNUSED(type);

	saveSensorConfiguration();
	updateGraphicWatchSensorsList();
}

utils::sensorsGraph::SensorsGraph *Interpreter::graphicsWatchWindow() const
{
	return mGraphicsWatch;
}
