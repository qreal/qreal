#include <QtGui/QAction>
#include "interpreter.h"

#include "details/autoconfigurer.h"
#include "details/robotImplementations/unrealRobotModelImplementation.h"
#include "details/robotCommunication/bluetoothRobotCommunicationThread.h"
#include "details/robotCommunication/usbRobotCommunicationThread.h"
#include "details/tracer.h"
#include "details/debugHelper.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

const Id startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");
const Id startingElementType1 = Id("RobotsMetamodel", "RobotsDiagram", "InitialBlock");

Interpreter::Interpreter()
	: mGraphicalModelApi(NULL)
	, mLogicalModelApi(NULL)
	, mInterpretersInterface(NULL)
	, mState(idle)
	, mRobotModel(new RobotModel())
	, mBlocksTable(NULL)
	, mRobotCommunication(new RobotCommunicator(SettingsManager::value("valueOfCommunication").toString()))
	, mImplementationType(robotModelType::null)
	, mWatchListWindow(NULL)
	, mActionConnectToRobot(NULL)
{
	mParser = NULL;
	mBlocksTable = NULL;
	mTimer = new QTimer();

	mD2RobotModel = new d2Model::D2RobotModel();
	mD2ModelWidget = mD2RobotModel->createModelWidget();

	connect(mRobotModel, SIGNAL(disconnected()), this, SLOT(disconnectSlot()));
	connect(mRobotModel, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
	connect(mRobotModel, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	connect(mD2ModelWidget, SIGNAL(d2WasClosed()), this, SLOT(stopRobot()));
}

void Interpreter::init(GraphicalModelAssistInterface const &graphicalModelApi
	, LogicalModelAssistInterface const &logicalModelApi
	, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
{
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mInterpretersInterface = &interpretersInterface;

	mParser = new RobotsBlockParser(mInterpretersInterface->errorReporter());
	mBlocksTable = new BlocksTable(graphicalModelApi, logicalModelApi, mRobotModel, mInterpretersInterface->errorReporter(), mParser);

	robotModelType::robotModelTypeEnum const modelType = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
	Tracer::debug(tracer::initialization, "Interpreter::init", "Going to set robot implementation, model type is " + DebugHelper::toString(modelType));
	setRobotImplementation(modelType);
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
	Tracer::debug(tracer::initialization, "Interpreter::interpret", "Preparing for interpretation");

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

	mState = waitingForSensorsConfiguredToLaunch;
	mBlocksTable->setIdleForBlocks();

	Id const startingElement = findStartingElement(currentDiagramId);
	if (startingElement == Id()) {
		mInterpretersInterface->errorReporter()->addError(tr("No entry point found, please add Initial Node to a diagram"));
		mState = idle;
		return;
	}

	Autoconfigurer configurer(*mGraphicalModelApi, mBlocksTable, mInterpretersInterface->errorReporter(), mRobotModel);
	if (!configurer.configure(currentDiagramId)) {
		return;
	}

	runTimer();
}

void Interpreter::stopRobot()
{
	mTimer->stop();
	mRobotModel->stopRobot();
	mState = idle;
	foreach (Thread *thread, mThreads) {
		delete thread;
		mThreads.removeAll(thread);
	}
	mBlocksTable->setFailure();
}

void Interpreter::showWatchList()
{
	mWatchListWindow = new watchListWindow(mParser);
	mWatchListWindow->show();
}

void Interpreter::closeWatchList()
{
	if (mWatchListWindow) {
		mWatchListWindow->setVisible(false);
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
		mD2ModelWidget->showNormal();
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

void Interpreter::setRobotImplementation(robotModelType::robotModelTypeEnum implementationType)
{
	mConnected = false;
	robotImplementations::AbstractRobotModelImplementation *robotImpl =
			robotImplementations::AbstractRobotModelImplementation::robotModel(implementationType, mRobotCommunication, mD2RobotModel);
	setRobotImplementation(robotImpl);
	mImplementationType = implementationType;
	if (mImplementationType != robotModelType::real) {
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
		Tracer::debug(tracer::initialization, "Interpreter::connectedSlot", "Robot connection status: " + QString::number(success));
		mInterpretersInterface->errorReporter()->addError(tr("Can't connect to a robot."));
	}
	mConnected = success;
	mActionConnectToRobot->setChecked(success);
}

void Interpreter::sensorsConfiguredSlot()
{
	Tracer::debug(tracer::initialization, "Interpreter::sensorsConfiguredSlot", "Sensors are configured");

	mConnected = true;
	mActionConnectToRobot->setChecked(mConnected);

	resetVariables();

	mRobotModel->nextBlockAfterInitial(mConnected);

	if (mState == waitingForSensorsConfiguredToLaunch) {
		mState = interpreting;

		Tracer::debug(tracer::initialization, "Interpreter::sensorsConfiguredSlot", "Starting interpretation");
		mRobotModel->startInterpretation();

		Id const &currentDiagramId = mInterpretersInterface->activeDiagram();
		Id const startingElement = findStartingElement(currentDiagramId);
		Thread * const initialThread = new Thread(*mInterpretersInterface, *mBlocksTable, startingElement);
		addThread(initialThread);
	}
}

Id const Interpreter::findStartingElement(Id const &diagram) const
{
	IdList const children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

	foreach (Id const child, children) {
		if (child.type() == startingElementType || child.type() == startingElementType1) {
			return child;
		}
	}

	return Id();
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
	Thread * const thread = new Thread(*mInterpretersInterface, *mBlocksTable, startBlock->id());
	addThread(thread);
}

void Interpreter::configureSensors(sensorType::SensorTypeEnum const &port1
		, sensorType::SensorTypeEnum const &port2
		, sensorType::SensorTypeEnum const &port3
		, sensorType::SensorTypeEnum const &port4)
{
	if (mConnected) {
		mRobotModel->configureSensors(port1, port2, port3, port4);
	}
}

void Interpreter::addThread(details::Thread * const thread)
{
	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));
	connect(thread, SIGNAL(newThread(details::blocks::Block*const)), this, SLOT(newThread(details::blocks::Block*const)));

	thread->interpret();
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
	mTimer->start(1000);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(readSensorValues()));
	if (mRobotModel->sensor(inputPort::port1)) {
		connect(mRobotModel->sensor(inputPort::port1)->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot1(int)));
		connect(mRobotModel->sensor(inputPort::port1)->sensorImpl(), SIGNAL(failure()), this, SLOT(slotFailure()));
	}
	if (mRobotModel->sensor(inputPort::port2)) {
		connect(mRobotModel->sensor(inputPort::port2)->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot2(int)));
		connect(mRobotModel->sensor(inputPort::port2)->sensorImpl(), SIGNAL(failure()), this, SLOT(slotFailure()));
	}
	if (mRobotModel->sensor(inputPort::port3)) {
		connect(mRobotModel->sensor(inputPort::port3)->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot3(int)));
		connect(mRobotModel->sensor(inputPort::port3)->sensorImpl(), SIGNAL(failure()), this, SLOT(slotFailure()));
	}
	if (mRobotModel->sensor(inputPort::port4)) {
		connect(mRobotModel->sensor(inputPort::port4)->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot4(int)));
		connect(mRobotModel->sensor(inputPort::port4)->sensorImpl(), SIGNAL(failure()), this, SLOT(slotFailure()));
	}
}

void Interpreter::readSensorValues()
{
	if (mState == idle) {
		return;
	}

	if (mRobotModel->sensor(inputPort::port1)) {
		mRobotModel->sensor(inputPort::port1)->read();
	}
	if (mRobotModel->sensor(inputPort::port2)) {
		mRobotModel->sensor(inputPort::port2)->read();
	}
	if (mRobotModel->sensor(inputPort::port3)) {
		mRobotModel->sensor(inputPort::port3)->read();
	}
	if (mRobotModel->sensor(inputPort::port4)) {
		mRobotModel->sensor(inputPort::port4)->read();
	}
}

void Interpreter::slotFailure()
{
	Tracer::debug(tracer::autoupdatedSensorValues, "Interpreter::slotFailure", "");
}

void Interpreter::responseSlot1(int sensorValue)
{
	updateSensorValues("Sensor1", sensorValue);
}

void Interpreter::responseSlot2(int sensorValue)
{
	updateSensorValues("Sensor2", sensorValue);
}

void Interpreter::responseSlot3(int sensorValue)
{
	updateSensorValues("Sensor3", sensorValue);
}

void Interpreter::responseSlot4(int sensorValue)
{
	updateSensorValues("Sensor4", sensorValue);
}

void Interpreter::updateSensorValues(QString const &sensorVariableName, int sensorValue)
{
	(*(mParser->getVariables()))[sensorVariableName] = utils::Number(sensorValue, utils::Number::intType);
	Tracer::debug(tracer::autoupdatedSensorValues, "Interpreter::updateSensorValues", sensorVariableName + QString::number(sensorValue));
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
	if (mConnected) {
		mRobotModel->stopRobot();
		mRobotModel->disconnectFromRobot();
	} else {
		mRobotModel->init();
		configureSensors(static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
						 , static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
						 , static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
						 , static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt()));
	}
	mActionConnectToRobot->setChecked(mConnected);
}

void Interpreter::disconnectSlot()
{
	mActionConnectToRobot->setChecked(false);
	mConnected = false;
}

void Interpreter::setRobotModelType(robotModelType::robotModelTypeEnum robotModelType)
{
	setRobotImplementation(robotModelType);
}

void Interpreter::setCommunicator(QString const &valueOfCommunication, QString const &portName)
{
	RobotCommunicationThreadInterface *communicator = NULL;
	if (valueOfCommunication == "bluetooth") {
		communicator = new BluetoothRobotCommunicationThread();
	} else {
		communicator = new UsbRobotCommunicationThread();
	}

	mRobotCommunication->setRobotCommunicationThreadObject(communicator);
	mRobotCommunication->setPortName(portName);
	connectToRobot();
}

void Interpreter::setConnectRobotAction(QAction *actionConnect)
{
	mActionConnectToRobot = actionConnect;
}
