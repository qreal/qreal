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

#include "kitBase/blocksBase/common/waitForColorBlock.h"

#include "kitBase/robotModel/robotParts/colorSensorFull.h"

using namespace kitBase;
using namespace blocksBase::common;

WaitForColorBlock::WaitForColorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

void WaitForColorBlock::responseSlot(int reading)
{
	const QString targetColor = stringProperty("Color");
	QString color;
	switch (reading) {
	case 1: color = "black";
		break;
	case 2: color = "blue";
		break;
	case 3: color = "green";
		break;
	case 4: color = "yellow";
		break;
	case 5: color = "red";
		break;
	case 6: color = "white";
		break;
	default:
		return;
	}

	if (targetColor == color) {
		stop();
	}
}

robotModel::DeviceInfo WaitForColorBlock::device() const
{
	return robotModel::DeviceInfo::create<robotModel::robotParts::ColorSensorFull>();
}
