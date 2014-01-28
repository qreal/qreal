#include "blocksTable.h"

using namespace qReal;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::baseBlocks;
using namespace interpreterCore::interpreter::details;

BlocksTable::BlocksTable(GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface const &logicalModelApi
		, RobotModelInterface * const robotModel
		, ErrorReporterInterface * const errorReporter
		, RobotsBlockParser * const parser
		, interpreterBase::baseBlocks::BlocksFactoryInterface * const blocksFactory
		)
		: mBlocksFactory(blocksFactory)
{
}

BlocksTable::~BlocksTable()
{
	qDeleteAll(mBlocks);
	delete mBlocksFactory;
}

BlockInterface *BlocksTable::block(Id const &element)
{
	if (mBlocks.contains(element)) {
		return mBlocks[element];
	}

	BlockInterface *newBlock = mBlocksFactory->block(element);
	addBlock(element, newBlock);
	return newBlock;
}

void BlocksTable::clear()
{
//	mBlocksFactory->getParser()->robotsClearVariables();
	qDeleteAll(mBlocks);
	mBlocks.clear();
}

void BlocksTable::setFailure()
{
	foreach (BlockInterface * const block, mBlocks.values()) {
		block->setFailedStatus();
	}
}

void BlocksTable::setIdleForBlocks()
{
	foreach (BlockInterface * const block, mBlocks.values()) {
		block->setIdleStatus();
	}
}

void BlocksTable::addBlock(Id const &element, BlockInterface *block)
{
	mBlocks.insert(element, block);
}

qReal::IdList BlocksTable::providedBlocks() const
{
	return mBlocksFactory->providedBlocks();
}
