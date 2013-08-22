#include "blocksTable.h"

//#include "blocks/block.h"
#include "blocksFactory.h"

using namespace qReal;
using namespace robotsInterpreterCore;

BlocksTable::BlocksTable(GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface const &logicalModelApi
		, RobotModelInterface * const robotModel
		, ErrorReporterInterface * const errorReporter
		, RobotsBlockParser * const parser
		)
		: mBlocksFactory(new BlocksFactory(graphicalModelApi, logicalModelApi, robotModel, errorReporter, this, parser))
{
}

BlocksTable::~BlocksTable()
{
	qDeleteAll(mBlocks);
	delete mBlocksFactory;
}

Block *BlocksTable::block(Id const &element)
{
	if (mBlocks.contains(element)) {
		return mBlocks[element];
	}

	Block *newBlock = mBlocksFactory->block(element);
	addBlock(element, newBlock);
	return newBlock;
}

void BlocksTable::clear()
{
	mBlocksFactory->getParser()->robotsClearVariables();
	qDeleteAll(mBlocks);
	mBlocks.clear();
}

void BlocksTable::setFailure()
{
	foreach (Block * const block, mBlocks.values()) {
		block->setFailedStatus();
	}
}

void BlocksTable::setIdleForBlocks()
{
	foreach (Block * const block, mBlocks.values()) {
		block->setIdleStatus();
	}
}

void BlocksTable::addBlock(Id const &element, Block *block)
{
	mBlocks.insert(element, block);
}

qReal::IdList BlocksTable::commonBlocks() const
{
	return mBlocksFactory->commonBlocks();
}
