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

#include "timerBlock.h"

#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>

using namespace interpreterCore::coreBlocks::details;

TimerBlock::TimerBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: mTimer(robotModel.timeline().produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &TimerBlock::timeout);
}

TimerBlock::~TimerBlock()
{
}

void TimerBlock::run()
{
	const int interval = eval<int>("Delay");
	if (!errorsOccured()) {
		mTimer->start(interval);
	}
}

void TimerBlock::timeout()
{
	mTimer->stop();
	emit done(mNextBlockId);
}
