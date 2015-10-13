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

#include "ev3EnginesBackwardBlock.h"

#include "ev3Kit/robotModel/parts/ev3Motor.h"

using namespace ev3::blocks::details;
using namespace ev3::robotModel::parts;

Ev3EnginesBackwardBlock::Ev3EnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void Ev3EnginesBackwardBlock::run()
{
	const int power = -eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	for (Ev3Motor * const motor : parsePorts<Ev3Motor>()) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
