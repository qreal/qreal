#include "ev3BlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>
#include <interpreterBase/blocksBase/common/clearEncoderBlock.h>

#include <interpreterBase/blocksBase/common/waitForColorBlock.h>
#include <interpreterBase/blocksBase/common/waitForColorIntensityBlock.h>
#include <interpreterBase/blocksBase/common/waitForEncoderBlock.h>
#include <interpreterBase/blocksBase/common/waitForLightSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <interpreterBase/blocksBase/common/waitForSoundSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForTouchSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForButtonBlock.h>

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

#include "details/beepBlock.h"
#include "details/playToneBlock.h"

#include "details/ev3EnginesForwardBlock.h"
#include "details/ev3EnginesBackwardBlock.h"

#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawCircleBlock.h"

using namespace ev3KitInterpreter::blocks;
using namespace details;
using namespace interpreterBase::blocksBase::common;

qReal::interpretation::Block *Ev3BlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "Ev3Beep")) {
		return new BeepBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3PlayTone")) {
		return new PlayToneBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3EnginesForward")) {
		return new details::Ev3EnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3EnginesBackward")) {
		return new details::Ev3EnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3EnginesStop")) {
		return new EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3ClearEncoder")) {
		return new ClearEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "Ev3WaitForTouchSensor")) {
		return new WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForRangeSensor")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, interpreterBase::robotModel::DeviceInfo::create<
						interpreterBase::robotModel::robotParts::RangeSensor>());
	} else if (elementMetatypeIs(element, "Ev3WaitForColor")) {
		return new WaitForColorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForColorIntensity")) {
		return new WaitForColorIntensityBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForLight")) {
		return new WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForSound")) {
		return new WaitForSoundSensorBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "Ev3WaitForUp")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "UpButtonPort");
	} else if (elementMetatypeIs(element, "Ev3WaitForEnter")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "EnterButtonPort");
	} else if (elementMetatypeIs(element, "Ev3WaitForDown")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "DownButtonPort");
	} else if (elementMetatypeIs(element, "Ev3WaitForRight")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "RightButtonPort");
	} else if (elementMetatypeIs(element, "Ev3WaitForLeft")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "LeftButtonPort");
	} else if (elementMetatypeIs(element, "Ev3WaitForBack")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "BackButtonPort");
	} else if (elementMetatypeIs(element, "Ev3DrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3DrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3DrawCircle")) {
		return new DrawCircleBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3DrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList Ev3BlocksFactory::providedBlocks() const
{
	return {
		id("Ev3Beep")
		, id("Ev3PlayTone")
		, id("Ev3EnginesForward")
		, id("Ev3EnginesBackward")
		, id("Ev3EnginesStop")
		, id("Ev3ClearEncoder")

		, id("Ev3WaitForTouchSensor")
		, id("Ev3WaitForRangeSensor")
		, id("Ev3WaitForColor")
		, id("Ev3WaitForColorIntensity")
		, id("Ev3WaitForEncoder")
		, id("Ev3WaitForUp")
		, id("Ev3WaitForEnter")
		, id("Ev3WaitForDown")
		, id("Ev3WaitForRight")
		, id("Ev3WaitForLeft")
		, id("Ev3WaitForBack")
		, id("Ev3WaitForSound")

		, id("Ev3DrawPixel")
		, id("Ev3DrawLine")
		, id("Ev3DrawCircle")
		, id("Ev3DrawRect")
	};
}

qReal::IdList Ev3BlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("Ev3WaitForSound")
				;
	}

	return result;
}
