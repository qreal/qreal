#include "interpreter.h"

#include "details/autoconfigurer.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace details;

const Id startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");

Interpreter::Interpreter(models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, RobotCommunicationInterface * const robotCommunicationInterface)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mState(idle)
	, mRobotModel(robotCommunicationInterface)
	, mBlocksTable(NULL)
{
	mBlocksTable = new BlocksTable(graphicalModelApi, logicalModelApi, &mRobotModel, mInterpretersInterface.errorReporter());
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
	mRobotModel.clear();
}

void Interpreter::stopRobot()
{
	mRobotModel.stopRobot();
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
	mRobotModel.configureSensors(port1, port2, port3, port4);
}

void Interpreter::addThread(details::Thread * const thread)
{
	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));
	connect(thread, SIGNAL(newThread(details::blocks::Block*const)), this, SLOT(newThread(details::blocks::Block*const)));

	thread->interpret();
}
