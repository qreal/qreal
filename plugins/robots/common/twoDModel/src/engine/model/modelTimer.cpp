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

#include "modelTimer.h"

using namespace twoDModel::model;

ModelTimer::ModelTimer(const Timeline *timeline)
	: mTimeline(timeline)
	, mTimeToWait(0)
	, mListening(false)
	, mTimePast(0)
	, mInterval(0)
	, mRepeatable(false)
{
	connect(timeline, SIGNAL(tick()), this, SLOT(onTick()));
}

bool ModelTimer::isTicking() const
{
	return mListening;
}

void ModelTimer::start()
{
	start(mInterval);
}

void ModelTimer::start(int ms)
{
	mInterval = ms;
	mTimeToWait = ms;
	mTimePast = 0;
	mListening = true;
}

void ModelTimer::stop()
{
	mTimeToWait = 0;
	mTimePast = 0;
	mListening = false;
}

void ModelTimer::onTick()
{
	if (!mListening) {
		return;
	}

	mTimePast += Timeline::timeInterval;
	if (mTimePast >= mTimeToWait) {
		mListening = false;
		onTimeout();
	}
}

void ModelTimer::setInterval(int ms)
{
	mInterval = ms;
}

void ModelTimer::setRepeatable(bool repeatable)
{
	mRepeatable = repeatable;
}

void ModelTimer::onTimeout()
{
	AbstractTimer::onTimeout();
	if (mRepeatable) {
		start();
	}
}

