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

#include "drawPixelBlock.h"

#include "ev3Kit/robotModel/parts/ev3Display.h"

using namespace ev3::blocks::details;

DrawPixelBlock::DrawPixelBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	const int x = eval<int>("XCoordinatePix");
	const int y = eval<int>("YCoordinatePix");
	const bool redraw = boolProperty("Redraw");

	if (!errorsOccured()) {
		ev3Display->drawPixel(x, y);
		if (redraw) {
			ev3Display->redraw();
		}

		emit done(mNextBlockId);
	}
}
