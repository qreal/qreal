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

#include "kitBase/blocksBase/common/waitForTouchSensorBlock.h"

#include "kitBase/robotModel/robotParts/touchSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForTouchSensorBlock::responseSlot(int reading)
{
	if (reading > 0) {
		stop();
	}
}

kitBase::robotModel::DeviceInfo WaitForTouchSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}
