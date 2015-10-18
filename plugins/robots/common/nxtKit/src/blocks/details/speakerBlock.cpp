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

#include "speakerBlock.h"

#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace nxt::blocks::details;
using namespace kitBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::NxtSpeaker>(robotModel)
	, mTimer(robotModel.timeline().produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &SpeakerBlock::timeout);
}

void SpeakerBlock::run()
{
	DeviceBlock::run();
	if (errorsOccured()) {
		return;
	}

	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlockId);
	else {
		mTimer->start(duration());
	}
}

void SpeakerBlock::timeout()
{
	emit done(mNextBlockId);
}
