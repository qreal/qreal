/* Copyright 2013-2016 CyberTech Labs Ltd., Grigorii Zimin
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

#include "kitBase/blocksBase/common/enginesStopBlock.h"

#include <kitBase/robotModel/robotParts/motor.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

EnginesStopBlock::EnginesStopBlock(RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesStopBlock::run()
{
	robotModel::robotParts::MotorsAggregator *aggregator = findMotorsAggregator();
	const QList<robotParts::Motor *> ports = parsePorts<robotParts::Motor>();
	if (aggregator) {
		QList<QString> portsNames;
		for (robotParts::Motor * const motor : ports) {
			portsNames.append(motor->port().name());
		}

		aggregator->off(portsNames);
	} else {
		for (robotParts::Motor * const motor : ports) {
			motor->off();
		}
	}

	emit done(mNextBlockId);
}
