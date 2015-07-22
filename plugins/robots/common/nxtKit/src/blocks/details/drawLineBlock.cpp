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

#include "drawLineBlock.h"

#include "nxtKit/robotModel/parts/nxtDisplay.h"

using namespace nxt::blocks::details;

DrawLineBlock::DrawLineBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawLineBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto nxtDisplay = static_cast<robotModel::parts::NxtDisplay *>(&display);
	const int x1 = eval<int>("X1CoordinateLine");
	const int y1 = eval<int>("Y1CoordinateLine");
	const int x2 = eval<int>("X2CoordinateLine");
	const int y2 = eval<int>("Y2CoordinateLine");
	const bool redraw = boolProperty("Redraw");

	if (!errorsOccured()) {
		nxtDisplay->drawLine(x1, y1, x2, y2);
		if (redraw) {
			nxtDisplay->redraw();
		}

		emit done(mNextBlockId);
	}
}
