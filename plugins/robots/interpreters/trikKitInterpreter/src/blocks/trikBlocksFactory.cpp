#include "trikBlocksFactory.h"

#include <interpreterBase/blocksBase/common/enginesStopBlock.h>
#include <interpreterBase/blocksBase/common/clearEncoderBlock.h>

#include <interpreterBase/blocksBase/common/waitForAccelerometerBlock.h>
#include <interpreterBase/blocksBase/common/waitForColorBlock.h>
#include <interpreterBase/blocksBase/common/waitForColorIntensityBlock.h>
#include <interpreterBase/blocksBase/common/waitForEncoderBlock.h>
#include <interpreterBase/blocksBase/common/waitForGyroscopeBlock.h>
#include <interpreterBase/blocksBase/common/waitForLightSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <interpreterBase/blocksBase/common/waitForSoundSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForTouchSensorBlock.h>

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

	} else if (elementMetatypeIs(element, "TrikWaitForLight")) {
		return new interpreterBase::blocksBase::common::WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForSonarDistance")) {
		return new interpreterBase::blocksBase::common::WaitForSonarDistanceBlock(mRobotModelManager->model());
	/// @todo: support TrikWaitForIRDistance.
//	} else if (elementMetatypeIs(element, "TrikWaitForIRDistance")) {
//		return new interpreterBase::blocksBase::common::WaitForSonarDistanceBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForGyroscope")) {
		return new interpreterBase::blocksBase::common::WaitForGyroscopeSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForAccelerometer")) {
		return new interpreterBase::blocksBase::common::WaitForAccelerometerSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForEncoder")) {
		return new interpreterBase::blocksBase::common::WaitForEncoderBlock(mRobotModelManager->model());

//	} else if (elementMetatypeIs(element, "TrikWaitForEnter")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "TrikWaitForLeft")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "TrikWaitForRight")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "TrikWaitForDown")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "TrikWaitForUp")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "TrikWaitForPower")) {
//		return new interpreterBase::blocksBase::common::WaitForSoundSensorBlock(mRobotModelManager->model());

	}

	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("V4")) {
		result
				<< id("TrikV4EnginesBackward")
				<< id("TrikV4EnginesForward")
				<< id("TrikV4EnginesStop")
				<< id("TrikV4ClearEncoder")
				;
	} else {
		result
				<< id("TrikV6EnginesBackward")
				<< id("TrikV6EnginesForward")
				<< id("TrikV6EnginesStop")
				<< id("TrikV6ClearEncoder")
				;
	}

//	result
//			<< id("TrikSay")
//			<< id("TrikLed")
//			<< id("TrikSystem")
//			<< id("TrikInitCamera")
//			<< id("TrikDetectLine")
//			<< id("TrikLineDetectorToVariable")
//			;

	result
			<< id("TrikWaitForLight")
			<< id("TrikWaitForSonarDistance")
//			<< id("TrikWaitForIRDistance")
			<< id("TrikWaitForGyroscope")
			<< id("TrikWaitForAccelerometer")
			<< id("TrikWaitForEncoder")
//			<< id("TrikWaitForEnter")
//			<< id("TrikWaitForLeft")
//			<< id("TrikWaitForRight")
//			<< id("TrikWaitForDown")
//			<< id("TrikWaitForUp")
//			<< id("TrikWaitForPower")
			;

//	result
//			<< id("TrikSmile")
//			<< id("TrikSadSmile")
//			<< id("TrikSetBackground")
//			;

	return result;
}

qReal::IdList TrikBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("TrikWaitForGyroscope")
				<< id("TrikWaitForAccelerometer")
				<< id("TrikSay")
				<< id("TrikLed")
				<< id("TrikSystem")
				<< id("TrikInitCamera")
				<< id("TrikDetectLine")
				<< id("TrikLineDetectorToVariable")
				;
	}

	return result;
}
