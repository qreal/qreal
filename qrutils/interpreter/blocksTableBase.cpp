#include "blocksTableBase.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace interpretation;

BlocksTableBase::BlocksTableBase()
{
}

BlocksTableBase::~BlocksTableBase()
{
	qDeleteAll(mBlocks);
}

BlockInterface *BlocksTableBase::block(const Id &element)
{
	if (mBlocks.contains(element)) {
		return mBlocks[element];
	}

	BlockInterface * const newBlock = produceBlock(element);
	if (newBlock) {
		mBlocks.insert(element, newBlock);
	}

	return newBlock;
}

void BlocksTableBase::clear()
{
	qDeleteAll(mBlocks);
	mBlocks.clear();
}

void BlocksTableBase::setFailure()
{
	for (BlockInterface * const block : mBlocks.values()) {
		block->setFailedStatus();
	}
}
