#include "nxtKit/blocks/nxtBlocksFactory.h"

#include <kitBase/blocksBase/common/enginesStopBlock.h>
#include <kitBase/blocksBase/common/clearEncoderBlock.h>

#include <kitBase/blocksBase/common/waitForColorBlock.h>
#include <kitBase/blocksBase/common/waitForColorIntensityBlock.h>
#include <kitBase/blocksBase/common/waitForEncoderBlock.h>
#include <kitBase/blocksBase/common/waitForLightSensorBlock.h>
#include <kitBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <kitBase/blocksBase/common/waitForSoundSensorBlock.h>
#include <kitBase/blocksBase/common/waitForTouchSensorBlock.h>
#include <kitBase/blocksBase/common/waitForButtonBlock.h>

#include <kitBase/robotModel/robotParts/rangeSensor.h>

#include "details/beepBlock.h"
#include "details/playToneBlock.h"

#include "details/nxtEnginesForwardBlock.h"
#include "details/nxtEnginesBackwardBlock.h"

#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawCircleBlock.h"

using namespace nxt::blocks;
using namespace details;
using namespace kitBase::blocksBase::common;

qReal::interpretation::Block *NxtBlocksFactory::produceBlock(const qReal::Id &element)
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
		return new EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtClearEncoder")) {
		return new ClearEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "NxtWaitForTouchSensor")) {
		return new WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForSonarDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, kitBase::robotModel::DeviceInfo::create<
						kitBase::robotModel::robotParts::RangeSensor>());
	} else if (elementMetatypeIs(element, "NxtWaitForColor")) {
		return new WaitForColorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForColorIntensity")) {
		return new WaitForColorIntensityBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForLight")) {
		return new WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtWaitForSound")) {
		return new WaitForSoundSensorBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "NxtWaitForButton")) {
		return new WaitForButtonBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawCircle")) {
		return new DrawCircleBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "NxtDrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
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
		, id("NxtWaitForButton")

		, id("NxtDrawPixel")
		, id("NxtDrawLine")
		, id("NxtDrawCircle")
		, id("NxtDrawRect")
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
