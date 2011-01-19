#include "blocksFactory.h"

#include "../../../kernel/exception/exception.h"
#include "blocks/simpleTestBlock.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

Block *BlocksFactory::block(Id const &element
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable
		)
{
	return new SimpleTestBlock(element, graphicalModelApi, blocksTable);
}
