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

#include "detectLineBlock.h"
#include "trikKit/robotModel/parts/trikObjectSensor.h"
#include "trikKit/robotModel/parts/trikLineSensor.h"
#include "trikKit/robotModel/parts/trikColorSensor.h"
#include "kitBase/robotModel/robotModelInterface.h"
#include "kitBase/robotModel/robotModelUtils.h"

using namespace trik::blocks::details;
using namespace trik::robotModel::parts;

DetectLineBlock::DetectLineBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<trik::robotModel::parts::TrikVideoSensor>(robotModel)
{
}

bool DetectLineBlock::runSpecific()
{
	const QString portVP1 = "VP1";
	const QString portVP2 = "VP2";

	TrikVideoSensor * const deviceVP1 =
			kitBase::robotModel::RobotModelUtils::findDevice<TrikVideoSensor>(getModel(), portVP1);
	if (deviceVP1) {
		doJob(*deviceVP1);
	}

	TrikVideoSensor * const deviceVP2 =
			kitBase::robotModel::RobotModelUtils::findDevice<TrikVideoSensor>(getModel(), portVP2);
	if (deviceVP2) {
		doJob(*deviceVP2);
	}

	return true;
}

void DetectLineBlock::doJob(trik::robotModel::parts::TrikVideoSensor &camera)
{
	camera.detect();
	emit done(mNextBlockId);
}
