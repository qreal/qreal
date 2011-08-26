#include "interpreter.h"

#include "details/autoconfigurer.h"
#include "details/robotImplementations/unrealRobotModelImplementation.h"

#include <QtCore/QDebug>

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
	, mBluetoothRobotCommunication(NULL)
{
	mParser = NULL;
	mBlocksTable = NULL;
	mTimer = new QTimer();

	mD2RobotModel = new d2Model::D2RobotModel();
	mD2ModelWidget = mD2RobotModel->createModelWidget();
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

	QString const comPort = SettingsManager::instance()->value("bluetoothPortName", "").toString();
	mBluetoothRobotCommunication = new BluetoothRobotCommunication(comPort);
	robotModelType::robotModelTypeEnum const modelType = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel", "1").toInt());
	setRobotImplementation(modelType, mBluetoothRobotCommunication);
}

Interpreter::~Interpreter()
{
	foreach (Thread * const thread, mThreads)
		delete thread;
	delete mBlocksTable;
}

void Interpreter::interpret()
{
	mInterpretersInterface->errorReporter()->clear();

	Id const &currentDiagramId = mInterpretersInterface->activeDiagram();

	if (!mConnected) {
		mInterpretersInterface->errorReporter()->addInformation(tr("No connection to robot"));
		return;
	}
	if (mState == interpreting) {
		mInterpretersInterface->errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	mState = interpreting;

	mBlocksTable->setIdleForBlocks();

	Id const startingElement = findStartingElement(currentDiagramId);
	if (startingElement == Id()) {
		mInterpretersInterface->errorReporter()->addError(tr("No entry point found, please add Initial Node to a diagram"));
		return;
	}

	Autoconfigurer configurer(*mGraphicalModelApi, mBlocksTable, mInterpretersInterface->errorReporter(), mRobotModel);
	if (!configurer.configure(currentDiagramId))
		return;

	mRobotModel->robotImpl().startInterpretation();

	Thread * const initialThread = new Thread(*mInterpretersInterface, *mBlocksTable, startingElement);
	addThread(initialThread);
}

void Interpreter::stop()
{
	mRobotModel->stopRobot();
	mState = idle;
	foreach (Thread *thread, mThreads) {
		delete thread;
		mThreads.removeAll(thread);
	}
	mBlocksTable->setFailure();
	/*mBlocksTable->clear();
	mThreads.clear();
	mTimer->stop();*/
}

void Interpreter::stopRobot()
{
	stop();
}

void Interpreter::showD2ModelWidget(bool isVisible)
{
	mD2ModelWidget->init(isVisible);
}

void Interpreter::setD2ModelWidgetActions(QAction *runAction, QAction *stopAction)
{
	mD2ModelWidget->setD2ModelWidgetActions(runAction, stopAction);
}

void Interpreter::setRobotImplementation(robotModelType::robotModelTypeEnum implementationType, RobotCommunicationInterface * const robotCommunicationInterface)
{
	disconnect(&mRobotModel->robotImpl(), SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	mConnected = false;
	robotImplementations::AbstractRobotModelImplementation *robotImpl =
			robotImplementations::AbstractRobotModelImplementation::robotModel(implementationType, robotCommunicationInterface, mD2RobotModel);
	setRobotImplementation(robotImpl);
	connect(robotImpl, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	if (implementationType != robotModelType::real)
		mRobotModel->init();
}

void Interpreter::connectedSlot(bool success)
{
	if (success) {
		mConnected = true;
		if (mRobotModel->robotImpl().needsConnection())
			mInterpretersInterface->errorReporter()->addInformation(tr("Connected successfully"));
	} else {
		mConnected = false;
		mInterpretersInterface->errorReporter()->addError(tr("Can't connect to a robot."));
	}
}

Id const Interpreter::findStartingElement(Id const &diagram) const
{
	IdList const children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

	foreach (Id const child, children) {
		if (child.type() == startingElementType || child.type() == startingElementType1)
			return child;
	}

	return Id();
}

void Interpreter::threadStopped()
{
	Thread *thread = static_cast<Thread *>(sender());

	mThreads.removeAll(thread);
	delete thread;

	if (mThreads.isEmpty())
		stop();
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
	mRobotModel->configureSensors(port1, port2, port3, port4);
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
	if (robotImpl)
		connect(&mRobotModel->robotImpl(), SIGNAL(connected(bool)), this, SLOT(runTimer()));
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
	if (mState == idle)
		return;

	if (mRobotModel->sensor(inputPort::port1))
		mRobotModel->sensor(inputPort::port1)->read();
	if (mRobotModel->sensor(inputPort::port2))
		mRobotModel->sensor(inputPort::port2)->read();
	if (mRobotModel->sensor(inputPort::port3))
		mRobotModel->sensor(inputPort::port3)->read();
	if (mRobotModel->sensor(inputPort::port4))
		mRobotModel->sensor(inputPort::port4)->read();
}

void Interpreter::slotFailure()
{
	qDebug() << "slotFailure";
}

void Interpreter::responseSlot1(int sensorValue)
{
	updateSensorValues(QObject::tr("Sensor1"), sensorValue);
}

void Interpreter::responseSlot2(int sensorValue)
{
	updateSensorValues(QObject::tr("Sensor2"), sensorValue);
}

void Interpreter::responseSlot3(int sensorValue)
{
	updateSensorValues(QObject::tr("Sensor3"), sensorValue);
}

void Interpreter::responseSlot4(int sensorValue)
{
	updateSensorValues(QObject::tr("Sensor4"), sensorValue);
}

void Interpreter::updateSensorValues(const QString &sensorVariableName, int sensorValue)
{
	(*(mParser->getVariables()))[sensorVariableName] = Number(sensorValue, Number::intType);
//	qDebug() << sensorVariableName << sensorValue;
}

void Interpreter::connectToRobot()
{
	mRobotModel->init();
}

void Interpreter::setBluetoothPortName(QString const &portName)
{
	mBluetoothRobotCommunication->setPortName(portName);
}

void Interpreter::setRobotModelType(robotModelType::robotModelTypeEnum robotModelType)
{
	setRobotImplementation(robotModelType, mBluetoothRobotCommunication);
}
