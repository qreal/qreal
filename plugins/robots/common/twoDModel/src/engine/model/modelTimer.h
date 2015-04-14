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

#pragma once

#include <utils/abstractTimer.h>

#include "timeline.h"

namespace twoDModel {
namespace model {

/// Timer implementation for 2D model. Used in TimerBlock and BeepBlock
class ModelTimer : public utils::AbstractTimer
{
	Q_OBJECT

public:
	explicit ModelTimer(const Timeline *timeline /* Doesn`t take ownership */);

	void start(int ms) override;
	void stop() override;

private slots:
	void onTick();

private:
	const Timeline *mTimeline;
	int mTimeToWait;
	bool mListening;
	int mTimePast;
};

}
}
