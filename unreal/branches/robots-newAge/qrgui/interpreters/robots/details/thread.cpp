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

void Thread::interpret()
{
	nextBlock(mCurrentBlock);
}

void Thread::nextBlock(blocks::Block * const block)
{
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
	mCurrentBlock->interpret();
}
