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
	if (element.type() == Id("RobotsMetamodel", "RobotsDiagram", "InitialNode"))
		return new InitialBlock(element, graphicalModelApi, blocksTable, *mRobotModel);
	if (element.type() == Id("RobotsMetamodel", "RobotsDiagram", "Beep"))
		return new BeepBlock(element, graphicalModelApi, blocksTable, mRobotModel->brick());
	if (element.type() == Id("RobotsMetamodel", "RobotsDiagram", "Timer"))
		return new TimerBlock(element, graphicalModelApi, logicalModelApi, blocksTable);

	return new DummyBlock(element, graphicalModelApi, logicalModelApi, blocksTable);
}
