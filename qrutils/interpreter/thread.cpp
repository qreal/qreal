#include "thread.h"

#include <QtWidgets/QApplication>
#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace interpretation;

const int blocksCountTillProcessingEvents = 100;

Thread::Thread(const GraphicalModelAssistInterface *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, const Id &initialNodeType
		, BlocksTableInterface &blocksTable
		, const Id &initialNode)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mInitialNodeType(initialNodeType)
	, mBlocksTable(blocksTable)
	, mCurrentBlock(mBlocksTable.block(initialNode))
	, mBlocksSincePreviousEventsProcessing(0)
	, mProcessEventsTimer(new QTimer(this))
	, mProcessEventsMapper(new QSignalMapper(this))
{
	initTimer();
}

Thread::Thread(const GraphicalModelAssistInterface *graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, const Id &initialNodeType
		, const Id &diagramToInterpret
		, BlocksTableInterface &blocksTable)
	: mGraphicalModelApi(graphicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mInitialNodeType(initialNodeType)
	, mBlocksTable(blocksTable)
	, mCurrentBlock(nullptr)
	, mInitialDiagram(diagramToInterpret)
	, mBlocksSincePreviousEventsProcessing(0)
	, mProcessEventsTimer(new QTimer(this))
	, mProcessEventsMapper(new QSignalMapper(this))
{
	initTimer();
}

Thread::~Thread()
{
	for (const BlockInterface * const block : mStack) {
		if (block) {
			mInterpretersInterface.dehighlight(block->id());
		}
	}
}

void Thread::initTimer()
{
	mProcessEventsTimer->setSingleShot(true);
	mProcessEventsTimer->setInterval(true);
	connect(mProcessEventsTimer, SIGNAL(timeout())
			, mProcessEventsMapper, SLOT(map()));

	connect(mProcessEventsMapper, SIGNAL(mapped(QObject*))
			, this, SLOT(interpretAfterEventsProcessing(QObject*)));
}

void Thread::interpret()
{
	if (mCurrentBlock) {
		turnOn(mCurrentBlock);
	} else {
		stepInto(mInitialDiagram);
	}
}

void Thread::nextBlock(const Id &blockId)
{
	turnOff(mCurrentBlock);
	BlockInterface *const block = blockId == Id() ? nullptr : mBlocksTable.block(blockId);
	turnOn(block);
}

void Thread::stepInto(const Id &diagram)
{
	const Id initialNode = findStartingElement(diagram);
	BlockInterface * const block = mBlocksTable.block(initialNode);

	if (initialNode.isNull() || !block) {
		error(tr("No entry point found, please add Initial Node to a diagram"), diagram);
		return;
	}

	if (mStack.count() >= SettingsManager::value("interpreterStackSize").toInt()) {
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

void Thread::error(const QString &message, const Id &source)
{
	mInterpretersInterface.errorReporter()->addError(message, source);
	failure();
}

Id Thread::findStartingElement(const Id &diagram) const
{
	const IdList children = mGraphicalModelApi->graphicalRepoApi().children(diagram);

	for (const Id &child : children) {
		if (child.type() == mInitialNodeType) {
			return child;
		}
	}

	return Id();
}

void Thread::turnOn(BlockInterface * const block)
{
	mCurrentBlock = block;
	if (!mCurrentBlock) {
		/// @todo: report error if we met unknown block type?
		finishedSteppingInto();
		return;
	}

	mInterpretersInterface.highlight(mCurrentBlock->id(), false);
	connect(mCurrentBlock, &BlockInterface::done, this, &Thread::nextBlock);
	connect(mCurrentBlock, &BlockInterface::newThread, this, &Thread::newThread);
	connect(mCurrentBlock, &BlockInterface::failure, this, &Thread::failure);
	connect(mCurrentBlock, &BlockInterface::stepInto, this, &Thread::stepInto);

	mStack.push(mCurrentBlock);

	++mBlocksSincePreviousEventsProcessing;
	if (mBlocksSincePreviousEventsProcessing > blocksCountTillProcessingEvents) {
		// Here we want to process all accumulated events and terminate blocks recursion
		mBlocksSincePreviousEventsProcessing = 0;

		// After timer was started control flow returns to event loop and
		// among events there can be the one that destroys this thread instance.
		// There are some suspicions that in QObject destructor children are removed
		// not immediately but with deleteLater() method. So there is a small probability
		// that slot will be triggered after this object is destroyed. Then
		// we can not access fields, but can access alive block instance in mapper.
		// Kind of hack, but quite an easy way not to suspend everything if
		// interpreted program has an infinite loop without timers.
		mProcessEventsMapper->removeMappings(mProcessEventsTimer);
		mProcessEventsMapper->setMapping(mProcessEventsTimer, mCurrentBlock);
		mProcessEventsTimer->start();
	} else {
		mCurrentBlock->interpret();
	}
}

void Thread::interpretAfterEventsProcessing(QObject *blockObject)
{
	BlockInterface * const block = dynamic_cast<BlockInterface *>(blockObject);
	if (block) {
		block->interpret();
	}
}

void Thread::turnOff(BlockInterface * const block)
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
