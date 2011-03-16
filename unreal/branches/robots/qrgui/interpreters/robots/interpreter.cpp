#include "interpreter.h"

#include "details/autoconfigurer.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace interpreters::robots;
using namespace details;

const Id startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");

Interpreter::Interpreter(models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, RobotModel * const robotModel)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mState(idle)
	, mRobotModel(robotModel)
	, mBlocksTable(NULL)
{
	mParser = new BlockParser(mInterpretersInterface.errorReporter());
	mBlocksTable = new BlocksTable(graphicalModelApi, logicalModelApi, mRobotModel, mInterpretersInterface.errorReporter(), mParser);
	mTimer = new QTimer();
}

Interpreter::~Interpreter()
{
	foreach (Thread * const thread, mThreads)
		delete thread;
	delete mBlocksTable;
}

void Interpreter::interpret(Id const &currentDiagramId)
{
	if (mState == interpreting) {
		mInterpretersInterface.errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	Id const startingElement = findStartingElement(currentDiagramId);
	if (startingElement == Id()) {
		mInterpretersInterface.errorReporter()->addError(tr("No entry point found, please add Initial Node to a diagram"));
		return;
	}

	Autoconfigurer configurer(mGraphicalModelApi, mBlocksTable, mInterpretersInterface.errorReporter(), mRobotModel);
	if (!configurer.configure(currentDiagramId))
		return;

	Thread * const initialThread = new Thread(mInterpretersInterface, *mBlocksTable, startingElement);
	addThread(initialThread);
}

void Interpreter::stop()
{
	mState = idle;
	foreach (Thread *thread, mThreads)
		delete thread;
	mBlocksTable->clear();
	mThreads.clear();
	mRobotModel->clear();
}

void Interpreter::stopRobot()
{
	mRobotModel->stopRobot();
	stop();
}

Id const Interpreter::findStartingElement(Id const &diagram) const
{
	IdList const children = mGraphicalModelApi.graphicalRepoApi().children(diagram);

	foreach (Id const child, children) {
		if (child.type() == startingElementType)
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
	Thread * const thread = new Thread(mInterpretersInterface, *mBlocksTable, startBlock->id());
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

details::RobotModel *Interpreter::robotModel()
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
	connect (mTimer, SIGNAL(timeout()), this, SLOT(readSensorValues()));
//	for (inputPort::InputPortEnum port = inputPort::port1; port <= inputPort::port4; ++port) {
//	}
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
	qDebug() << "FAILURE, just lol";
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

void Interpreter::updateSensorValues(const QString &sensorVariableName, int sensorValue)
{
	mParser->getVariables()[sensorVariableName] = Number(sensorValue, Number::intType);
	qDebug() << sensorVariableName << sensorValue;
}

