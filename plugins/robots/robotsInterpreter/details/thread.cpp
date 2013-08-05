#include "thread.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

Id const startingElementType = Id("RobotsMetamodel", "RobotsDiagram", "InitialNode");

Thread::Thread(GraphicalModelAssistInterface const *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, BlocksTable &blocksTable, Id const &initialNode)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mBlocksTable(blocksTable)
	, mCurrentBlock(mBlocksTable.block(initialNode))
{
}

Thread::Thread(GraphicalModelAssistInterface const *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, Id const &diagramToInterpret, BlocksTable &blocksTable)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mBlocksTable(blocksTable)
	, mCurrentBlock(NULL)
	, mInitialDiagram(diagramToInterpret)
{
}

Thread::~Thread()
{
	foreach (blocks::Block *block, mStack) {
		if (block) {
			mInterpretersInterface.dehighlight(block->id());
		}
	}
}

void Thread::interpret()
{
	if (mCurrentBlock) {
		nextBlock(mCurrentBlock);
	} else {
		stepInto(mInitialDiagram);
	}
}

void Thread::nextBlock(blocks::Block * const block)
{
	turnOff(mCurrentBlock);
	turnOn(block);
}

void Thread::stepInto(Id const &diagram)
{
	Id const initialNode = findStartingElement(diagram);
	blocks::Block *block = mBlocksTable.block(initialNode);

	if (!block) {
		error(tr("No entry point found, please add Initial Node to a diagram"), diagram);
		return;
	}

	if (mStack.count() >= SettingsManager::value("robotsInterpreterStackSize").toInt()) {
		error(tr("Stack overflow"));
		return;
	}

	turnOn(block);
}

void Thread::finishedSteppingInto()
{
	if (mStack.isEmpty()) {
		emit stopped();
		return;
	}
	mCurrentBlock = mStack.top();
	// Execution must proceed here
	mCurrentBlock->finishedSteppingInto();
}

void Thread::failure()
{
	emit stopped();
}

void Thread::error(QString const &message, Id const &source)
{
	mInterpretersInterface.errorReporter()->addError(message, source);
	failure();
}

Id Thread::findStartingElement(Id const &diagram) const
{
	IdList const children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

	foreach (Id const &child, children) {
		if (child.type() == startingElementType) {
			return child;
		}
	}

	return Id();
}

void Thread::turnOn(blocks::Block * const block)
{
	mCurrentBlock = block;
	if (!mCurrentBlock) {
		finishedSteppingInto();
		return;
	}

	mInterpretersInterface.highlight(mCurrentBlock->id(), false);
	connect(mCurrentBlock, SIGNAL(done(blocks::Block * const)), this, SLOT(nextBlock(blocks::Block * const)));
	connect(mCurrentBlock, SIGNAL(newThread(details::blocks::Block*const)), this, SIGNAL(newThread(details::blocks::Block*const)));
	connect(mCurrentBlock, SIGNAL(failure()), this, SLOT(failure()));
	connect(mCurrentBlock, SIGNAL(stepInto(Id const &)), this, SLOT(stepInto(Id const &)));

	mStack.push(mCurrentBlock);
	mCurrentBlock->interpret();
}

void Thread::turnOff(blocks::Block * const block)
{
	// This is a signal not from a current block of this thread.
	// Other thread shall process it, we will just ignore.
	if (sender() && sender() != block) {
		return;
	}

	if (sender()) {
		sender()->disconnect(this);
	}

	mStack.pop();
	mInterpretersInterface.dehighlight(block->id());
}
