/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "trikKit/blocks/trikBlocksFactory.h"

#include <kitBase/blocksBase/common/enginesStopBlock.h>
#include <kitBase/blocksBase/common/clearEncoderBlock.h>

#include <kitBase/blocksBase/common/waitForAccelerometerBlock.h>
#include <kitBase/blocksBase/common/waitForEncoderBlock.h>
#include <kitBase/blocksBase/common/waitForGyroscopeSensorBlock.h>
#include <kitBase/blocksBase/common/waitForLightSensorBlock.h>
#include <kitBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <kitBase/blocksBase/common/waitForButtonBlock.h>
#include <kitBase/blocksBase/common/getButtonCodeBlock.h>

#include <qrutils/interpreter/blocks/emptyBlock.h>
#include "details/smileBlock.h"
#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawEllipseBlock.h"
#include "details/drawArcBlock.h"
#include "details/setPainterWidthBlock.h"
#include "details/setPainterColorBlock.h"
#include "details/setBackgroundBlock.h"
#include "details/trikEnginesBackwardBlock.h"
#include "details/trikEnginesForwardBlock.h"
#include "details/initCameraBlock.h"
#include "details/detectLineBlock.h"
#include "details/lineDetectorToVariable.h"
#include "details/waitForMotionBlock.h"
#include "details/speakerBlock.h"
#include "details/ledBlock.h"
#include "details/sayBlock.h"
#include "details/systemCommandBlock.h"

#include "details/waitGamepadButtonBlock.h"
#include "details/waitGamepadConnectBlock.h"
#include "details/waitGamepadDisconnectBlock.h"
#include "details/waitGamepadWheelBlock.h"
#include "details/waitPadPressBlock.h"

#include "details/writeToFileBlock.h"
#include "details/removeFileBlock.h"

#include "trikKit/robotModel/parts/trikInfraredSensor.h"

using namespace trik::blocks;
using namespace trik::blocks::details;
using namespace kitBase::blocksBase::common;

qReal::interpretation::Block *TrikBlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "TrikPlayTone")) {
		return new SpeakerBlock(mRobotModelManager->model());
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
		return new SayBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikSystem")) {
		return new SystemCommandBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikInitCamera")) {
		return new InitCameraBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDetect")) {
		return new DetectLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDetectorToVariable")) {
		return new LineDetectorToVariableBlock();
	} else if (elementMetatypeIs(element, "TrikSendMessage")) {
		return new qReal::interpretation::blocks::EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikWaitForMessage")) {
		return new qReal::interpretation::blocks::EmptyBlock();

	} else if (elementMetatypeIs(element, "TrikLed")) {
		return new LedBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForLight")) {
		return new WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForSonarDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, kitBase::robotModel::DeviceInfo::create<
						kitBase::robotModel::robotParts::RangeSensor>());
	} else if (elementMetatypeIs(element, "TrikWaitForIRDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikInfraredSensor>());
	} else if (elementMetatypeIs(element, "TrikWaitForGyroscope")) {
		return new WaitForGyroscopeSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForAccelerometer")) {
		return new WaitForAccelerometerSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForMotion")) {
		return new WaitForMotionBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "TrikWaitForButton")) {
		return new WaitForButtonBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikSetPainterColor")) {
		return new SetPainterColorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikSetPainterWidth")) {
		return new SetPainterWidthBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawEllipse")) {
		return new DrawEllipseBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikDrawArc")) {
		return new DrawArcBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikSmile")) {
		return new SmileBlock(mRobotModelManager->model(), false);
	} else if (elementMetatypeIs(element, "TrikSadSmile")) {
		return new SmileBlock(mRobotModelManager->model(), true);
	} else if (elementMetatypeIs(element, "TrikSetBackground")) {
		return new SetBackgroundBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "TrikWaitGamepadButton")) {
		return new WaitGamepadButtonBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikWaitPadPress")) {
		return new qReal::interpretation::blocks::EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikWaitGamepadWheel")) {
		return new qReal::interpretation::blocks::EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikWaitGamepadDisconnect")) {
		return new qReal::interpretation::blocks::EmptyBlock();
	} else if (elementMetatypeIs(element, "TrikWaitGamepadConnect")) {
		return new qReal::interpretation::blocks::EmptyBlock();

	} else if (elementMetatypeIs(element, "TrikWriteToFile")) {
		return new WriteToFileBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikRemoveFile")) {
		return new RemoveFileBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "GetButtonCode")) {
		return new GetButtonCodeBlock(mRobotModelManager->model());
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
			<< id("TrikDetect")
			<< id("TrikDetectorToVariable")
			<< id("TrikSendMessage")
			<< id("TrikWaitForMessage")
			;

	result
			<< id("TrikWaitForLight")
			<< id("TrikWaitForSonarDistance")
			<< id("TrikWaitForIRDistance")
			<< id("TrikWaitForGyroscope")
			<< id("TrikWaitForAccelerometer")
			<< id("TrikWaitForMotion")
			<< id("TrikWaitForEncoder")
			<< id("TrikWaitForButton")
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

	result
			<< id("TrikWaitGamepadButton")
			<< id("TrikWaitPadPress")
			<< id("TrikWaitGamepadWheel")
			<< id("TrikWaitGamepadDisconnect")
			<< id("TrikWaitGamepadConnect")
			;

	result
			<< id("TrikWriteToFile")
			<< id("TrikRemoveFile")
			;

	result << id("GetButtonCode");

	return result;
}

qReal::IdList TrikBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("TrikWaitForGyroscope")
				<< id("TrikWaitForAccelerometer")
				<< id("TrikSystem")
				<< id("TrikWaitForMotion")
				<< id("TrikSendMessage")
				<< id("TrikWaitForMessage")
				<< id("TrikWaitGamepadButton")
				<< id("TrikWaitPadPress")
				<< id("TrikWaitGamepadWheel")
				<< id("TrikWaitGamepadDisconnect")
				<< id("TrikWaitGamepadConnect")
				;
	}

	return result;
}
