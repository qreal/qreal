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

#include "setBackgroundBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel::robotParts;

SetBackgroundBlock::SetBackgroundBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetBackgroundBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->setBackground(propertyToColor(stringProperty("Color")));
	if (boolProperty("Redraw")) {
		trikDisplay->redraw();
	}

	emit done(mNextBlockId);
}
