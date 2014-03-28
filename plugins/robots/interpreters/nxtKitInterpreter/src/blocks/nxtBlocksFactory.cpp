#include "nxtBlocksFactory.h"

#include "details/beepBlock.h"
#include "details/playToneBlock.h"

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
		, id("WaitForButtons")
		, id("DrawPixel")
		, id("DrawLine")
		, id("DrawCircle")
		, id("PrintText")
		, id("DrawRect")
		, id("ClearScreen")
	};
}
