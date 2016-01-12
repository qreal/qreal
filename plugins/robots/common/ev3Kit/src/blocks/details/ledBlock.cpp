/* Copyright 2015 CyberTech Labs Ltd.
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

#include "ledBlock.h"

using namespace ev3;
using namespace blocks;
using namespace details;
using namespace kitBase::robotModel;

LedBlock::LedBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Ev3Led>(robotModel)
{
}

void LedBlock::doJob(robotModel::parts::Ev3Led &led)
{
	led.setColor(toLedColor(stringProperty("Color")));
	emit done(mNextBlockId);
}

robotModel::parts::Ev3LedColor LedBlock::toLedColor(const QString &color) const
{
	if (color == "black") {
		return robotModel::parts::Ev3LedColor::off;
	}

	if (color == "red") {
		return robotModel::parts::Ev3LedColor::red;
	}

	if (color == "green") {
		return robotModel::parts::Ev3LedColor::green;
	}

	if (color == "orange") {
		return robotModel::parts::Ev3LedColor::orange;
	}

	if (color == "red_flash") {
		return robotModel::parts::Ev3LedColor::redFlash;
	}

	if (color == "green_flash") {
		return robotModel::parts::Ev3LedColor::greenFlash;
	}

	if (color == "orange_flash") {
		return robotModel::parts::Ev3LedColor::orangeFlash;
	}

	if (color == "red_pulse") {
		return robotModel::parts::Ev3LedColor::redPulse;
	}

	if (color == "green_pulse") {
		return robotModel::parts::Ev3LedColor::greenPulse;
	}

	if (color == "orange_pulse") {
		return robotModel::parts::Ev3LedColor::orangePulse;
	}

	return robotModel::parts::Ev3LedColor::off;
}
