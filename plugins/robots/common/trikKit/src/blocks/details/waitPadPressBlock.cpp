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

#include "waitPadPressBlock.h"

#include "trikKit/robotModel/parts/trikGamepadPadPressSensor.h"

using namespace trik::blocks::details;

WaitPadPressBlock::WaitPadPressBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitPadPressBlock::responseSlot(int reading)
{
	if (reading == 1) {
		stop();
	}
}

QString WaitPadPressBlock::port()
{
	const int result = eval<int>("Pad");
	switch (result) {
	case 1:
		return "GamepadPad1PressedPort";
	case 2:
		return "GamepadPad2PressedPort";
	}

	return "";
}

kitBase::robotModel::DeviceInfo WaitPadPressBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadPadPressSensor>();
}
