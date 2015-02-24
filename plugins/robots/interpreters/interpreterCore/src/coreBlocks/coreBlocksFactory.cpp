#include "coreBlocksFactory.h"

#include <qrutils/interpreter/blocks/initialBlock.h>
#include <qrutils/interpreter/blocks/finalBlock.h>

#include "details/timerBlock.h"
#include <qrutils/interpreter/blocks/functionBlock.h>
#include <qrutils/interpreter/blocks/variableInitBlock.h>

#include <qrutils/interpreter/blocks/loopBlock.h>
#include <qrutils/interpreter/blocks/forkBlock.h>
#include <qrutils/interpreter/blocks/ifBlock.h>
#include <qrutils/interpreter/blocks/switchBlock.h>
#include <qrutils/interpreter/blocks/commentBlock.h>
#include <qrutils/interpreter/blocks/subprogramBlock.h>

#include "details/printTextBlock.h"
#include "details/clearScreenBlock.h"

#include <commonTwoDModel/blocks/markerDownBlock.h>
#include <commonTwoDModel/blocks/markerUpBlock.h>

using namespace interpreterCore::coreBlocks;

qReal::interpretation::Block *CoreBlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "InitialNode")) {
		return new qReal::interpretation::blocks::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		return new qReal::interpretation::blocks::FinalBlock();
	} else if (elementMetatypeIs(element, "Timer")) {
		return new details::TimerBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "CommentBlock")) {
		return new qReal::interpretation::blocks::CommentBlock;
	} else if (elementMetatypeIs(element, "IfBlock")) {
		return new qReal::interpretation::blocks::IfBlock();
	} else if (elementMetatypeIs(element, "SwitchBlock")) {
		return new qReal::interpretation::blocks::SwitchBlock();
	} else if (elementMetatypeIs(element, "Loop")) {
		return new qReal::interpretation::blocks::LoopBlock();
	} else if (elementMetatypeIs(element, "Fork")) {
		return new qReal::interpretation::blocks::ForkBlock();
	} else if (elementMetatypeIs(element, "Subprogram")) {
		return new qReal::interpretation::blocks::SubprogramBlock();
	} else if (elementMetatypeIs(element, "Function")) {
		return new qReal::interpretation::blocks::FunctionBlock();
	} else if (elementMetatypeIs(element, "VariableInit")) {
		return new qReal::interpretation::blocks::VariableInitBlock();
	} else if (elementMetatypeIs(element, "PrintText")) {
		return new details::PrintTextBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "ClearScreen")) {
		return new details::ClearScreenBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "MarkerDown")) {
		return new twoDModel::blocks::MarkerDownBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "MarkerUp")) {
		return new twoDModel::blocks::MarkerUpBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList CoreBlocksFactory::providedBlocks() const
{
	return {
		id("ControlFlow")
		, id("InitialNode")
		, id("FinalNode")
		, id("Timer")
		, id("CommentBlock")
		, id("IfBlock")
		, id("SwitchBlock")
		, id("Loop")
		, id("Fork")
		, id("Join")
		, id("SendMessageThreads")
		, id("ReceiveMessageThreads")
		, id("Subprogram")
		, id("Function")
		, id("VariableInit")
		, id("ClearScreen")
		, id("PrintText")
		, id("MarkerDown")
		, id("MarkerUp")
	};
}

qReal::IdList CoreBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (!mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("MarkerDown")
				<< id("MarkerUp")
				;
	}

	return result;
}
