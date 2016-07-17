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

#include "nxtEnginesBackwardBlock.h"

#include "nxtKit/robotModel/parts/nxtMotor.h"

using namespace nxt::blocks::details;
using namespace nxt::robotModel::parts;

NxtEnginesBackwardBlock::NxtEnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void NxtEnginesBackwardBlock::run()
{
	const int power = -eval<int>("Power");

	if (errorsOccured()) {
		return;
	}

	const bool breakMode = stringProperty("Mode") != "float";

	for (NxtMotor * const motor : parsePorts<NxtMotor>()) {
		motor->on(power, breakMode);
	}

	emit done(mNextBlockId);
}
