#include "trikBlocksFactory.h"

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
	}

	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	return { id("TrikV4EnginesBackward")
			, id("TrikV6EnginesBackward")
			, id("TrikV4EnginesForward")
			, id("TrikV6EnginesForward")
			};
}
