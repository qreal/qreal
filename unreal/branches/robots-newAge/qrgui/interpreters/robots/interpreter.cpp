#include "interpreter.h"

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
	mBlocksTable = new BlocksTable(graphicalModelApi, logicalModelApi, &mRobotModel);
}

Interpreter::~Interpreter()
{
	foreach (Thread * const thread, mThreads)
		delete thread;
	delete mBlocksTable;
}

void Interpreter::interpret(Id const &currentDiagramId)
{
	if (mState == interpreting)
		return;

	Id const startingElement = findStartingElement(currentDiagramId);
	if (startingElement == Id())
		return;

	Thread * const initialThread = new Thread(mInterpretersInterface, *mBlocksTable, startingElement);
	mThreads.append(initialThread);
	connect(initialThread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	foreach (Thread * const thread, mThreads)
		thread->interpret();
}

void Interpreter::stop()
{
	mState = idle;
	mBlocksTable->clear();
	foreach (Thread *thread, mThreads) {
		delete thread;
	}
	mThreads.clear();
	mRobotModel.clear();
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
