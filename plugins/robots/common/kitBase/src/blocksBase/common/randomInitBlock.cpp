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

#include "kitBase/blocksBase/common/randomInitBlock.h"

#include <kitBase/robotModel/robotParts/random.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

RandomInitBlock::RandomInitBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void RandomInitBlock::run()
{
	int from = eval<int>("LowerBound");
	int to = eval<int>("UpperBound");

	const auto random = RobotModelUtils::findDevice<robotParts::Random>(mRobotModel, "RandomPort");

	const int result = random->random(from, to);

	evalCode(stringProperty("Variable") + " = " + QString::number(result));
	if (!errorsOccured()) {
		emit done(mNextBlockId);
	}
}
