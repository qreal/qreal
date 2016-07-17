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

#include "ev3Kit/blocks/ev3BlocksFactory.h"

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
#include "details/ledBlock.h"

#include "details/ev3EnginesForwardBlock.h"
#include "details/ev3EnginesBackwardBlock.h"

#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawCircleBlock.h"

using namespace ev3::blocks;
using namespace details;
using namespace kitBase::blocksBase::common;

Ev3BlocksFactory::Ev3BlocksFactory(const QStringList &interpretedModels)
	: mInterpretedModels(interpretedModels)
{
}

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
	} else if (elementMetatypeIs(element, "Ev3Led")) {
		return new LedBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "Ev3WaitForTouchSensor")) {
		return new WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "Ev3WaitForSonarDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, kitBase::robotModel::DeviceInfo::create<
						kitBase::robotModel::robotParts::RangeSensor>());
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

	} else if (elementMetatypeIs(element, "Ev3WaitForButton")) {
		return new WaitForButtonBlock(mRobotModelManager->model());
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
		, id("Ev3Led")

		, id("Ev3WaitForTouchSensor")
		, id("Ev3WaitForSonarDistance")
		, id("Ev3WaitForLight")
		, id("Ev3WaitForColor")
		, id("Ev3WaitForColorIntensity")
		, id("Ev3WaitForEncoder")
		, id("Ev3WaitForButton")
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
	} else {
		if (!mInterpretedModels.contains(mRobotModelManager->model().robotId())) {
			result << id("Join") << id("SendMessageThreads") << id("ReceiveMessageThreads") << id("KillThread");
		}
	}

	return result;
}
