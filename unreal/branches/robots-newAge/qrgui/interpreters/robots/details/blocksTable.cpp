#include "blocksTable.h"

#include "blocks/block.h"
#include "blocksFactory.h"

using namespace qReal;
using namespace interpreters::robots::details;

BlocksTable::BlocksTable(models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
{
}

BlocksTable::~BlocksTable()
{
	foreach (blocks::Block *block, mBlocks.values())
		delete block;
}

blocks::Block *BlocksTable::block(Id const &element)
{
	if (mBlocks.contains(element))
		return mBlocks[element];

	blocks::Block *newBlock = BlocksFactory::block(element, mGraphicalModelApi, mLogicalModelApi, *this);
	mBlocks.insert(element, newBlock);
	return newBlock;
}

void BlocksTable::clear()
{
	mBlocks.clear();
}
