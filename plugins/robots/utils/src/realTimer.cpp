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

#include <utils/realTimer.h>

using namespace utils;

RealTimer::RealTimer()
{
	setRepeatable(false);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

bool RealTimer::isTicking() const
{
	return mTimer.isActive();
}

void RealTimer::start()
{
	start(mTimer.interval());
}

void RealTimer::start(int ms)
{
	mTimer.setInterval(ms);
	mTimer.start();
}

void RealTimer::stop()
{
	mTimer.stop();
}

void RealTimer::setInterval(int ms)
{
	mTimer.setInterval(ms);
}

void RealTimer::setRepeatable(bool repeatable)
{
	mTimer.setSingleShot(!repeatable);
}
