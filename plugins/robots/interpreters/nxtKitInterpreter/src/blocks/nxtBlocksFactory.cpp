#include "nxtBlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>

#include "details/beepBlock.h"
#include "details/playToneBlock.h"

#include "details/nxtEnginesForwardBlock.h"
#include "details/nxtEnginesBackwardBlock.h"

#include "details/clearScreenBlock.h"
#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawCircleBlock.h"
#include "details/printTextBlock.h"

#include "details/waitForButtonsBlock.h"

using namespace nxtKitInterpreter::blocks;
using namespace details;

interpreterBase::blocksBase::Block *NxtBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "Beep")) {
		return new BeepBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "PlayTone")) {
		return new PlayToneBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesForward")) {
		return new details::NxtEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesBackward")) {
		return new details::NxtEnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesStop")) {
		return new interpreterBase::blocksBase::common::EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "WaitForButtons")) {
		return new WaitForButtonsBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "DrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "DrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "DrawCircle")) {
		return new DrawCircleBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "PrintText")) {
		return new PrintTextBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "DrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "ClearScreen")) {
		return new ClearScreenBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList NxtBlocksFactory::providedBlocks() const
{
	return {
		id("Beep")
		, id("PlayTone")
		, id("NxtEnginesForward")
		, id("NxtEnginesBackward")
		, id("NxtEnginesStop")
		, id("WaitForButtons")
		, id("DrawPixel")
		, id("DrawLine")
		, id("DrawCircle")
		, id("PrintText")
		, id("DrawRect")
		, id("ClearScreen")
	};
}
