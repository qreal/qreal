#include "nxtBlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>
#include <interpreterBase/blocksBase/common/clearEncoderBlock.h>

#include <interpreterBase/blocksBase/common/waitForColorBlock.h>
#include <interpreterBase/blocksBase/common/waitForColorIntensityBlock.h>
#include <interpreterBase/blocksBase/common/waitForEncoderBlock.h>
#include <interpreterBase/blocksBase/common/waitForLightSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <interpreterBase/blocksBase/common/waitForSoundSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForTouchSensorBlock.h>

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
	if (elementMetatypeIs(element, "NxtBeep")) {
		return new BeepBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtPlayTone")) {
		return new PlayToneBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesForward")) {
		return new details::NxtEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesBackward")) {
		return new details::NxtEnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtEnginesStop")) {
		return new interpreterBase::blocksBase::common::EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtClearEncoder")) {
		return new interpreterBase::blocksBase::common::ClearEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "NxtWaitForTouchSensor")) {
		return new interpreterBase::blocksBase::common::WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForSonarDistance")) {
		return new interpreterBase::blocksBase::common::WaitForSonarDistanceBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForColor")) {
		return new interpreterBase::blocksBase::common::WaitForColorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForEncoder")) {
		return new interpreterBase::blocksBase::common::WaitForEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForColorIntensity")) {
		return new interpreterBase::blocksBase::common::WaitForColorIntensityBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForLight")) {
		return new interpreterBase::blocksBase::common::WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForSound")) {
		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "NxtWaitForButtons")) {
		return new WaitForButtonsBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "DrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawLine")) {
		return new DrawCircleBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtPrintText")) {
		return new PrintTextBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtClearScreen")) {
		return new ClearScreenBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList NxtBlocksFactory::providedBlocks() const
{
	return {
		id("NxtBeep")
		, id("NxtPlayTone")
		, id("NxtEnginesForward")
		, id("NxtEnginesBackward")
		, id("NxtEnginesStop")
		, id("NxtClearEncoder")

		, id("NxtWaitForTouchSensor")
		, id("NxtWaitForSonarDistance")
		, id("NxtWaitForColor")
		, id("NxtWaitForColorIntensity")
		, id("NxtWaitForEncoder")
		, id("NxtWaitForLight")
		, id("NxtWaitForSound")
		, id("NxtWaitForButtons")

		, id("NxtDrawPixel")
		, id("NxtDrawLine")
		, id("NxtDrawCircle")
		, id("NxtPrintText")
		, id("NxtDrawRect")
		, id("NxtClearScreen")
	};
}

qReal::IdList NxtBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("NxtWaitForSound")
				;
	}

	return result;
}
