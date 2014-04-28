#include "trikBlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>

#include "details/trikEnginesBackwardBlock.h"
#include "details/trikEnginesForwardBlock.h"

using namespace trikKitInterpreter::blocks;

interpreterBase::blocksBase::Block *TrikBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "TrikV4EnginesBackward")
			|| elementMetatypeIs(element, "TrikV6EnginesBackward"))
	{
		return new details::TrikEnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV4EnginesForward")
			|| elementMetatypeIs(element, "TrikV6EnginesForward"))
	{
		return new details::TrikEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV4EnginesStop")
			|| elementMetatypeIs(element, "TrikV6EnginesStop"))
	{
		return new interpreterBase::blocksBase::common::EnginesStopBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	if (mRobotModelManager->model().name().contains("V4")) {
		return { id("TrikV4EnginesBackward"), id("TrikV4EnginesForward"), id("TrikV4EnginesStop") };
	} else {
		return { id("TrikV6EnginesBackward"), id("TrikV6EnginesForward"), id("TrikV6EnginesStop") };
	}
}
