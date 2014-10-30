#include "coreBlocksFactory.h"

#include "details/initialBlock.h"
#include "details/finalBlock.h"

#include "details/timerBlock.h"
#include "details/functionBlock.h"
#include "details/variableInitBlock.h"

#include "details/loopBlock.h"
#include "details/forkBlock.h"
#include "details/ifBlock.h"
#include "details/switchBlock.h"
#include "details/commentBlock.h"
#include "details/subprogramBlock.h"

#include "details/printTextBlock.h"
#include "details/clearScreenBlock.h"

#include <commonTwoDModel/blocks/prepareMarkerBlock.h>
#include <commonTwoDModel/blocks/removeMarkerBlock.h>

using namespace interpreterCore::coreBlocks;

interpreterBase::blocksBase::Block *CoreBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "InitialNode")) {
		return new details::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		return new details::FinalBlock();
	} else if (elementMetatypeIs(element, "Timer")) {
		return new details::TimerBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "CommentBlock")) {
		return new details::CommentBlock;
	} else if (elementMetatypeIs(element, "IfBlock")) {
		return new details::IfBlock();
	} else if (elementMetatypeIs(element, "SwitchBlock")) {
		return new details::SwitchBlock();
	} else if (elementMetatypeIs(element, "Loop")) {
		return new details::LoopBlock();
	} else if (elementMetatypeIs(element, "Fork")) {
		return new details::ForkBlock();
	} else if (elementMetatypeIs(element, "Subprogram")) {
		return new details::SubprogramBlock();
	} else if (elementMetatypeIs(element, "Function")) {
		return new details::FunctionBlock();
	} else if (elementMetatypeIs(element, "VariableInit")) {
		return new details::VariableInitBlock();
	} else if (elementMetatypeIs(element, "PrintText")) {
		return new details::PrintTextBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "ClearScreen")) {
		return new details::ClearScreenBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "PrepareMarker")) {
		return new twoDModel::blocks::PrepareMarkerBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "RemoveMarker")) {
		return new twoDModel::blocks::RemoveMarkerBlock(mRobotModelManager->model());
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
		, id("Subprogram")
		, id("Function")
		, id("VariableInit")
		, id("ClearScreen")
		, id("PrintText")
		, id("PrepareMarker")
		, id("RemoveMarker")
	};
}

qReal::IdList CoreBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (!mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("PrepareMarker")
				<< id("RemoveMarker")
				;
	}

	return result;
}
