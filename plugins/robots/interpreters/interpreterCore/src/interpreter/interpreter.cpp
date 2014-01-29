#include "interpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>

#include <interpreterBase/robotModel/robotModelInterface.h>

#include "details/autoconfigurer.h"
//#include "details/tracer.h"
//#include "details/debugHelper.h"

using namespace qReal;
using namespace interpreterCore::interpreter;

int const maxThreadsCount = 100;

Interpreter::Interpreter(GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, qReal::ProjectManagementInterface const &projectManager
		, interpreterBase::blocksBase::BlocksFactoryInterface * const blocksFactory
		, interpreterBase::robotModel::RobotModelInterface * const robotModel
		, QAction &connectToRobotAction
		)
	: mGraphicalModelApi(&graphicalModelApi)
	, mLogicalModelApi(&logicalModelApi)
	, mInterpretersInterface(&interpretersInterface)
	, mState(idle)
	, mRobotModel(robotModel)
	, mBlocksTable(nullptr)
	, mParser(new details::RobotsBlockParser(interpretersInterface.errorReporter()))
	, mActionConnectToRobot(connectToRobotAction)
{
	mBlocksTable = new details::BlocksTable(blocksFactory);

	connect(
			&mRobotModel->configuration()
			, &interpreterBase::robotModel::ConfigurationInterface::allDevicesConfigured
			, this
			, &Interpreter::sensorsConfiguredSlot
			);

	connect(
			mRobotModel
			, &interpreterBase::robotModel::RobotModelInterface::connected
			, this
			, &Interpreter::connectedSlot
			);

	connect(&projectManager, &qReal::ProjectManagementInterface::beforeOpen, this, &Interpreter::stopRobot);

	mRobotModel->init();
}

Interpreter::~Interpreter()
{
	qDeleteAll(mThreads);
	delete mBlocksTable;
}

interpreterBase::blocksBase::BlockParserInterface &Interpreter::parser() const
{
	return *mParser;
}

void Interpreter::interpret()
{
//	Tracer::debug(tracer::enums::initialization, "Interpreter::interpret", "Preparing for interpretation");

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

	details::Autoconfigurer configurer(*mGraphicalModelApi, mBlocksTable, mInterpretersInterface->errorReporter(), mRobotModel);

	bool configurationSucceeded = false;
//	QVector<interpreterBase::robotModel::SensorId> const sensorConfiguration
//			= configurer.configure(currentDiagramId, &configurationSucceeded);

	if (!configurationSucceeded) {
		return;
	}

//	mRobotModel->configureSensors(
//			sensorConfiguration[0]
//			, sensorConfiguration[1]
//			, sensorConfiguration[2]
//			, sensorConfiguration[3]
//			);
}

void Interpreter::stopRobot()
{
	mRobotModel->stopRobot();
	mState = idle;
	qDeleteAll(mThreads);
	mThreads.clear();
	mBlocksTable->setFailure();
}

void Interpreter::connectedSlot(bool success)
{
	if (success) {
		if (mRobotModel->needsConnection()) {
			mInterpretersInterface->errorReporter()->addInformation(tr("Connected successfully"));
		}
	} else {
//		Tracer::debug(tracer::enums::initialization, "Interpreter::connectedSlot", "Robot connection status: " + QString::number(success));
		mInterpretersInterface->errorReporter()->addError(tr("Can't connect to a robot."));
	}

	mConnected = success;
	mActionConnectToRobot.setChecked(success);
}

void Interpreter::sensorsConfiguredSlot()
{
//	Tracer::debug(tracer::enums::initialization, "Interpreter::sensorsConfiguredSlot", "Sensors are configured");

	mConnected = true;
	mActionConnectToRobot.setChecked(mConnected);

//	resetVariables();

//	mRobotModel->nextBlockAfterInitial(mConnected);

	if (mState == waitingForSensorsConfiguredToLaunch) {
		mState = interpreting;

//		runTimer();

//		Tracer::debug(tracer::enums::initialization, "Interpreter::sensorsConfiguredSlot", "Starting interpretation");
//		mRobotModel->startInterpretation();

		Id const &currentDiagramId = mInterpretersInterface->activeDiagram();

		details::Thread * const initialThread = new details::Thread(mGraphicalModelApi
				, *mInterpretersInterface, currentDiagramId, *mBlocksTable);

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
	details::Thread * const thread = new details::Thread(mGraphicalModelApi
			, *mInterpretersInterface, *mBlocksTable, startBlockId);

	addThread(thread);
}

//void Interpreter::configureSensors(
//		robots::enums::sensorType::SensorTypeEnum const &port1
//		, robots::enums::sensorType::SensorTypeEnum const &port2
//		, robots::enums::sensorType::SensorTypeEnum const &port3
//		, robots::enums::sensorType::SensorTypeEnum const &port4)
//{
//	if (mConnected) {
//		mRobotModel->configureSensors(port1, port2, port3, port4);
//	}
//
//	updateGraphicWatchSensorsList();
//}

void Interpreter::addThread(details::Thread * const thread)
{
	if (mThreads.count() >= maxThreadsCount) {
//		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
//		stopRobot();
	}

	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	connect(thread
			, SIGNAL(newThread(Id const &))
			, this, SLOT(newThread(Id const &))
			);

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
	}
}

//interpreters::robots::details::RobotModel *Interpreter::robotModel()
//{
//	return mRobotModel;
//}

//void Interpreter::setRobotModel(details::RobotModel * const robotModel)
//{
//	mRobotModel = robotModel;
//}

//void Interpreter::setRobotImplementation(details::robotImplementations::AbstractRobotModelImplementation *robotImpl)
//{
//	mRobotModel->setRobotImplementation(robotImpl);
//	if (robotImpl) {
//		connect(mRobotModel, SIGNAL(connected(bool)), this, SLOT(runTimer()));
//	}
//}

//void Interpreter::runTimer()
//{
//	if (mRobotModel->sensor(robots::enums::inputPort::port1)) {
//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port1)->sensorImpl()
//				, SIGNAL(response(int))
//				, this
//				, SLOT(responseSlot1(int))
//				, Qt::UniqueConnection
//				);

//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port1)->sensorImpl()
//				, SIGNAL(failure())
//				, this
//				, SLOT(slotFailure())
//				, Qt::UniqueConnection
//				);
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port2)) {
//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port2)->sensorImpl()
//				, SIGNAL(response(int))
//				, this
//				, SLOT(responseSlot2(int))
//				, Qt::UniqueConnection
//				);

//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port2)->sensorImpl()
//				, SIGNAL(failure())
//				, this
//				, SLOT(slotFailure())
//				, Qt::UniqueConnection
//				);
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port3)) {
//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port3)->sensorImpl()
//				, SIGNAL(response(int))
//				, this
//				, SLOT(responseSlot3(int))
//				, Qt::UniqueConnection
//				);

//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port3)->sensorImpl()
//				, SIGNAL(failure())
//				, this
//				, SLOT(slotFailure())
//				, Qt::UniqueConnection
//				);
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port4)) {
//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port4)->sensorImpl()
//				, SIGNAL(response(int))
//				, this
//				, SLOT(responseSlot4(int))
//				, Qt::UniqueConnection
//				);

//		connect(
//				mRobotModel->sensor(robots::enums::inputPort::port4)->sensorImpl()
//				, SIGNAL(failure())
//				, this
//				, SLOT(slotFailure())
//				, Qt::UniqueConnection
//				);
//	}

//	connect(mRobotModel->encoderA().encoderImpl(), SIGNAL(response(int)), this, SLOT(responseSlotA(int)), Qt::UniqueConnection);
//	connect(mRobotModel->encoderA().encoderImpl(), SIGNAL(failure()), this, SLOT(slotFailure()), Qt::UniqueConnection);
//	connect(mRobotModel->encoderB().encoderImpl(), SIGNAL(response(int)), this, SLOT(responseSlotB(int)), Qt::UniqueConnection);
//	connect(mRobotModel->encoderB().encoderImpl(), SIGNAL(failure()), this, SLOT(slotFailure()), Qt::UniqueConnection);
//	connect(mRobotModel->encoderC().encoderImpl(), SIGNAL(response(int)), this, SLOT(responseSlotC(int)), Qt::UniqueConnection);
//	connect(mRobotModel->encoderC().encoderImpl(), SIGNAL(failure()), this, SLOT(slotFailure()), Qt::UniqueConnection);

//	mRobotModel->nullifySensors();
//	if (!mTimer.isActive()) {
//		readSensorValues();
//		mTimer.start(25);
//		connect(&mTimer, SIGNAL(timeout()), this, SLOT(readSensorValues()), Qt::UniqueConnection);
//	}
//}

//void Interpreter::readSensorValues()
//{
//	if (mState == idle) {
//		return;
//	}

//	if (mRobotModel->sensor(robots::enums::inputPort::port1)) {
//		mRobotModel->sensor(robots::enums::inputPort::port1)->read();
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port2)) {
//		mRobotModel->sensor(robots::enums::inputPort::port2)->read();
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port3)) {
//		mRobotModel->sensor(robots::enums::inputPort::port3)->read();
//	}
//	if (mRobotModel->sensor(robots::enums::inputPort::port4)) {
//		mRobotModel->sensor(robots::enums::inputPort::port4)->read();
//	}

//	mRobotModel->encoderA().read();
//	mRobotModel->encoderB().read();
//	mRobotModel->encoderC().read();
//}

//void Interpreter::slotFailure()
//{
//	Tracer::debug(tracer::enums::autoupdatedSensorValues, "Interpreter::slotFailure", "");
//}

//void Interpreter::responseSlot1(int sensorValue)
//{
//	updateSensorValues("Sensor1", sensorValue);
//}

//void Interpreter::responseSlot2(int sensorValue)
//{
//	updateSensorValues("Sensor2", sensorValue);
//}

//void Interpreter::responseSlot3(int sensorValue)
//{
//	updateSensorValues("Sensor3", sensorValue);
//}

//void Interpreter::responseSlot4(int sensorValue)
//{
//	updateSensorValues("Sensor4", sensorValue);
//}

//void Interpreter::responseSlotA(int encoderValue)
//{
//	updateSensorValues("EncoderA", encoderValue);
//}

//void Interpreter::responseSlotB(int encoderValue)
//{
//	updateSensorValues("EncoderB", encoderValue);
//}

//void Interpreter::responseSlotC(int encoderValue)
//{
//	updateSensorValues("EncoderC", encoderValue);
//}

//void Interpreter::updateSensorValues(QString const &sensorVariableName, int sensorValue)
//{
//	(*(mParser->getVariables()))[sensorVariableName] = utils::Number(sensorValue, utils::Number::intType);
//	Tracer::debug(
//			tracer::enums::autoupdatedSensorValues
//			, "Interpreter::updateSensorValues"
//			, sensorVariableName + QString::number(sensorValue)
//			);
//}

//void Interpreter::resetVariables()
//{
//	int const resetValue = 0;
//	responseSlot1(resetValue);
//	responseSlot2(resetValue);
//	responseSlot3(resetValue);
//	responseSlot4(resetValue);
//}

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
//		configureSensors(
//				  static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
//				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
//				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
//				, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt()));
	}

	mActionConnectToRobot.setChecked(mConnected);
}

void Interpreter::disconnectSlot()
{
	mActionConnectToRobot.setChecked(false);
	mConnected = false;
}

//void Interpreter::setRobotModelType(robots::enums::robotModelType::robotModelTypeEnum robotModelType)
//{
//	setRobotImplementation(robotModelType);
//}

//void Interpreter::setCommunicator(QString const &valueOfCommunication)
//{
//	if (valueOfCommunication == mLastCommunicationValue) {
//		return;
//	}

//	RobotCommunicationThreadInterface *communicator = NULL;
//	if (valueOfCommunication == "bluetooth") {
//		communicator = new BluetoothRobotCommunicationThread();
//	} else if (valueOfCommunication == "usb") {
//		communicator = new UsbRobotCommunicationThread();
//	} else {
//		communicator = new TcpRobotCommunicationThread();
//	}

//	mLastCommunicationValue = valueOfCommunication;

//	mRobotCommunication->setRobotCommunicationThreadObject(communicator);
//}

//void Interpreter::setConnectRobotAction(QAction *actionConnect)
//{
//	mActionConnectToRobot = actionConnect;
//}

//void Interpreter::setNoiseSettings()
//{
//	mD2RobotModel->setNoiseSettings();
//	emit noiseSettingsChanged();
//}

//void Interpreter::reportError(QString const &message)
//{
//	mInterpretersInterface->errorReporter()->addError(message);
//}

//void Interpreter::on2dModelChanged(QDomDocument const &xml)
//{
//	Id const currentDiagramId = mInterpretersInterface->activeDiagram();
//	Id const logicalId = mGraphicalModelApi->logicalId(currentDiagramId);
//	if (logicalId != Id() && logicalId != Id::rootId()) {
//		mLogicalModelApi->setPropertyByRoleName(logicalId, xml.toString(4), "worldModel");
//	}
//}

//void Interpreter::saveSensorConfiguration()
//{
//	Id const currentDiagramId = mInterpretersInterface->activeDiagram();
//	Id const logicalId = mGraphicalModelApi->logicalId(currentDiagramId);
//	if (logicalId != Id() && logicalId != Id::rootId()) {
//		int const sensor1Value = SettingsManager::value("port1SensorType").toInt();
//		int const sensor2Value = SettingsManager::value("port2SensorType").toInt();
//		int const sensor3Value = SettingsManager::value("port3SensorType").toInt();
//		int const sensor4Value = SettingsManager::value("port4SensorType").toInt();
//		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor1Value), "sensor1Value");
//		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor2Value), "sensor2Value");
//		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor3Value), "sensor3Value");
//		mLogicalModelApi->setPropertyByRoleName(logicalId, QString::number(sensor4Value), "sensor4Value");
//	}
//}

//void Interpreter::loadSensorConfiguration(Id const &diagramId)
//{
//	int const oldSensor1Value = SettingsManager::value("port1SensorType").toInt();
//	int const oldSensor2Value = SettingsManager::value("port2SensorType").toInt();
//	int const oldSensor3Value = SettingsManager::value("port3SensorType").toInt();
//	int const oldSensor4Value = SettingsManager::value("port4SensorType").toInt();

//	int const sensor1Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor1Value").toInt();
//	int const sensor2Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor2Value").toInt();
//	int const sensor3Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor3Value").toInt();
//	int const sensor4Value = mLogicalModelApi->propertyByRoleName(diagramId, "sensor4Value").toInt();

//	bool const somethingChanged = oldSensor1Value != sensor1Value
//			|| oldSensor2Value != sensor2Value
//			|| oldSensor3Value != sensor3Value
//			|| oldSensor4Value != sensor4Value;

//	SettingsManager::setValue("port1SensorType", sensor1Value);
//	SettingsManager::setValue("port2SensorType", sensor2Value);
//	SettingsManager::setValue("port3SensorType", sensor3Value);
//	SettingsManager::setValue("port4SensorType", sensor4Value);

//	if (somethingChanged) {
//		emit sensorsConfigurationChanged();
//	}
//}

//utils::WatchListWindow *Interpreter::watchWindow() const
//{
//	return mWatchListWindow;
//}

//void Interpreter::connectSensorConfigurer(details::SensorsConfigurationWidget *configurer) const
//{
//	connect(configurer, SIGNAL(saved()), mD2ModelWidget, SLOT(syncronizeSensors()));
//}

qReal::IdList Interpreter::providedBlocks() const
{
	return mBlocksTable->providedBlocks();
}

//void Interpreter::updateGraphicWatchSensorsList()
//{
//	mGraphicsWatch->addTrackingObject(0, QString("Sensor1")
//			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
//					(SettingsManager::instance()->value("port1SensorType").toInt())));
//	mGraphicsWatch->addTrackingObject(1, QString("Sensor2")
//			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
//					(SettingsManager::instance()->value("port2SensorType").toInt())));
//	mGraphicsWatch->addTrackingObject(2, QString("Sensor3")
//			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
//					(SettingsManager::instance()->value("port3SensorType").toInt())));
//	mGraphicsWatch->addTrackingObject(3, QString("Sensor4")
//			, SensorEnumerator::sensorName(static_cast<robots::enums::sensorType::SensorTypeEnum>
//					(SettingsManager::instance()->value("port4SensorType").toInt())));
//}

//utils::sensorsGraph::SensorsGraph *Interpreter::graphicsWatchWindow() const
//{
//	return mGraphicsWatch;
//}
