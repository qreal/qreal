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

#include "beepBlock.h"

#include <utils/abstractTimer.h>

using namespace ev3::blocks::details;

BeepBlock::BeepBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: SpeakerBlock(robotModel)
{
}

BeepBlock::~BeepBlock()
{
}

int BeepBlock::duration()
{
	return 500;
}

void BeepBlock::doJob(ev3::robotModel::parts::Ev3Speaker &speaker)
{
	int const volume = eval<int>("Volume");
	speaker.beep(volume, duration());
}
