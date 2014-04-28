#include "trikBlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>
#include <interpreterBase/blocksBase/common/clearEncoderBlock.h>

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
	} else if (elementMetatypeIs(element, "TrikV4ClearEncoder")
			|| elementMetatypeIs(element, "TrikV6ClearEncoder"))
	{
		return new interpreterBase::blocksBase::common::ClearEncoderBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	if (mRobotModelManager->model().name().contains("V4")) {
		return { id("TrikV4EnginesBackward")
				, id("TrikV4EnginesForward")
				, id("TrikV4EnginesStop")
				, id("TrikV4ClearEncoder")
				};
	} else {
		return { id("TrikV6EnginesBackward")
				, id("TrikV6EnginesForward")
				, id("TrikV6EnginesStop")
				, id("TrikV6ClearEncoder")
				};
	}
}

qReal::IdList TrikBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	result << id("WaitForTouchSensor")
			<< id("WaitForColor")
			<< id("WaitForColorIntensity")
			<< id("WaitForSound")
			;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result << id("WaitForGyroscope")
				;
	}

	return result;
}
