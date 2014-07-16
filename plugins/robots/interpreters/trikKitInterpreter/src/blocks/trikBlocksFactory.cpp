#include "trikBlocksFactory.h"

#include <interpreterBase/blocksBase/common/emptyBlock.h>
#include <interpreterBase/blocksBase/common/enginesStopBlock.h>
#include <interpreterBase/blocksBase/common/clearEncoderBlock.h>

#include <interpreterBase/blocksBase/common/waitForAccelerometerBlock.h>
#include <interpreterBase/blocksBase/common/waitForEncoderBlock.h>
#include <interpreterBase/blocksBase/common/waitForGyroscopeSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForLightSensorBlock.h>
#include <interpreterBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <interpreterBase/blocksBase/common/waitForButtonBlock.h>

#include "details/smileBlock.h"
#include "details/drawLineBlock.h"
#include "details/setBackgroundBlock.h"
#include "details/trikEnginesBackwardBlock.h"
#include "details/trikEnginesForwardBlock.h"
#include "details/waitForMotionBlock.h"
#include "robotModel/parts/trikInfraredSensor.h"

using namespace trikKitInterpreter::blocks;
using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::blocksBase::common;

interpreterBase::blocksBase::Block *TrikBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "TrikPlayTone")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikV4EnginesBackward")
			|| elementMetatypeIs(element, "TrikV6EnginesBackward"))
	{
		return new details::TrikEnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV4EnginesForward")
			|| elementMetatypeIs(element, "TrikV6EnginesForward")
			|| elementMetatypeIs(element, "TrikAngularServo"))
	{
		// AngularServo and EnginesForward are synonyms since angular and radial servos are controlled the same way.
		return new details::TrikEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV4EnginesStop")
			|| elementMetatypeIs(element, "TrikV6EnginesStop"))
	{
		return new EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV4ClearEncoder")
			|| elementMetatypeIs(element, "TrikV6ClearEncoder"))
	{
		return new ClearEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "TrikSay")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikLed")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikSystem")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikInitCamera")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikDetectLine")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikLineDetectorToVariable")) {
		return new EmptyBlock();

	} else if (elementMetatypeIs(element, "TrikWaitForLight")) {
		return new WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForSonarDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, interpreterBase::robotModel::DeviceInfo::create<
						interpreterBase::robotModel::robotParts::RangeSensor>());
	} else if (elementMetatypeIs(element, "TrikWaitForIRDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikInfraredSensor>());
	} else if (elementMetatypeIs(element, "TrikWaitForGyroscope")) {
		return new WaitForGyroscopeSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForAccelerometer")) {
		return new WaitForAccelerometerSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForMotion")) {
		return new WaitForMotionBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "TrikWaitForEnter")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "EnterButtonPort");
	} else if (elementMetatypeIs(element, "TrikWaitForLeft")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "LeftButtonPort");
	} else if (elementMetatypeIs(element, "TrikWaitForRight")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "RightButtonPort");
	} else if (elementMetatypeIs(element, "TrikWaitForDown")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "DownButtonPort");
	} else if (elementMetatypeIs(element, "TrikWaitForUp")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "UpButtonPort");
	} else if (elementMetatypeIs(element, "TrikWaitForPower")) {
		return new WaitForButtonBlock(mRobotModelManager->model(), "PowerButtonPort");

	} else if (elementMetatypeIs(element, "TrikSetPainterColor")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikSetPainterWidth")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikDrawPixel")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikDrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawRect")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikDrawEllipse")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikDrawArc")) {
		return new EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikSmile")) {
		return new SmileBlock(mRobotModelManager->model(), false);
	} else if (elementMetatypeIs(element, "TrikSadSmile")) {
		return new SmileBlock(mRobotModelManager->model(), true);
	} else if (elementMetatypeIs(element, "TrikSetBackground")) {
		return new SetBackgroundBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList TrikBlocksFactory::providedBlocks() const
{
	qReal::IdList result;

	result << id("TrikPlayTone");

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

	result
			<< id("TrikAngularServo")
			<< id("TrikSay")
			<< id("TrikLed")
			<< id("TrikSystem")
			<< id("TrikInitCamera")
			<< id("TrikDetectLine")
			<< id("TrikLineDetectorToVariable")
			;

	result
			<< id("TrikWaitForLight")
			<< id("TrikWaitForSonarDistance")
			<< id("TrikWaitForIRDistance")
			<< id("TrikWaitForGyroscope")
			<< id("TrikWaitForAccelerometer")
			<< id("TrikWaitForMotion")
			<< id("TrikWaitForEncoder")
			<< id("TrikWaitForEnter")
			<< id("TrikWaitForLeft")
			<< id("TrikWaitForRight")
			<< id("TrikWaitForDown")
			<< id("TrikWaitForUp")
			<< id("TrikWaitForPower")
			;

	result
			<< id("TrikSetPainterColor")
			<< id("TrikSetPainterWidth")
			<< id("TrikDrawPixel")
			<< id("TrikDrawLine")
			<< id("TrikDrawRect")
			<< id("TrikDrawEllipse")
			<< id("TrikDrawArc")
			<< id("TrikSmile")
			<< id("TrikSadSmile")
			<< id("TrikSetBackground")
			;

	return result;
}

qReal::IdList TrikBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("TrikPlayTone")
				<< id("TrikWaitForGyroscope")
				<< id("TrikWaitForAccelerometer")
				<< id("TrikSay")
				<< id("TrikLed")
				<< id("TrikSystem")
				<< id("TrikInitCamera")
				<< id("TrikDetectLine")
				<< id("TrikLineDetectorToVariable")
				<< id("TrikWaitForMotion")
				;
	} else {
		result
			<< id("Fork")
		;
	}

	return result;
}
