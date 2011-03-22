#include "thread.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace details;

Thread::Thread(gui::MainWindowInterpretersInterface &interpretersInterface, BlocksTable &blocksTable
		, Id const &initialNode)
	: mInterpretersInterface(interpretersInterface)
	, mCurrentBlock(NULL)
	, mBlocksTable(blocksTable)
{
	mCurrentBlock = blocksTable.block(initialNode);
}

Thread::~Thread()
{
	if (mCurrentBlock != NULL)
		mInterpretersInterface.dehighlight(mCurrentBlock->id());
}

void Thread::interpret()
{
	nextBlock(mCurrentBlock);
}

void Thread::nextBlock(blocks::Block * const block)
{
	// This is a signal not from a current block of this thread.
	// Other thread shall process it, we will just ignore.
	if (sender() != NULL && sender() != mCurrentBlock)
		return;

	if (sender() != NULL)
		disconnect(sender());

	mInterpretersInterface.dehighlight(mCurrentBlock->id());

	mCurrentBlock = block;
	if (!mCurrentBlock) {
		emit stopped();
		return;
	}

	mInterpretersInterface.highlight(mCurrentBlock->id(), false);
	connect(mCurrentBlock, SIGNAL(done(blocks::Block * const)), this, SLOT(nextBlock(blocks::Block * const)));
	connect(mCurrentBlock, SIGNAL(newThread(details::blocks::Block*const)), this, SIGNAL(newThread(details::blocks::Block*const)));
	connect(mCurrentBlock, SIGNAL(failure()), this, SLOT(failure()));
	qDebug() <<"before mCurrentBlock->interpret();";
	mCurrentBlock->interpret();
	qDebug() <<"after mCurrentBlock->interpret();";
}

void Thread::failure()
{
	emit stopped();
}
