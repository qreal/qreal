#include "blocksFactory.h"

#include "../../../kernel/exception/exception.h"
#include "blocks/timerBlock.h"
#include "blocks/beepBlock.h"
#include "blocks/initialBlock.h"
#include "blocks/dummyBlock.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

BlocksFactory::BlocksFactory(RobotModel * const robotModel)
	: mRobotModel(robotModel)
{
}

Block *BlocksFactory::block(Id const &element
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable
		)
{
	Block * newBlock = NULL;
	if (elementMetatypeIs(element, "InitialNode"))
		newBlock = new InitialBlock(*mRobotModel);
	else if (elementMetatypeIs(element, "Beep"))
		newBlock = new BeepBlock(mRobotModel->brick());
	else if (elementMetatypeIs(element, "Timer"))
		newBlock =new TimerBlock();
	else
		newBlock = new DummyBlock();

	newBlock->init(element, graphicalModelApi, logicalModelApi, blocksTable);
	return newBlock;
}

bool BlocksFactory::elementMetatypeIs(Id const &element, QString const &metatype)
{
	return element.type() == Id("RobotsMetamodel", "RobotsDiagram", metatype);
}

