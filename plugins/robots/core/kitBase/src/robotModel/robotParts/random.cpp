/* Copyright 2015 CyberTech Labs Ltd.
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

#include "kitBase/robotModel/robotParts/random.h"

using namespace kitBase::robotModel::robotParts;

Random::Random(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port)
	: Device(info, port)
{
}

int Random::random(int from, int to)
{
	++mCallsCount;

	if (mIsFixed) {
		mLastValue = mFixedValue;
		return mFixedValue;
	}

	if (from > to) {
		qSwap(from, to);
	}

	mLastValue = from + qrand() % (to - from + 1);
	return mLastValue;
}

void Random::setFixedValue(int value)
{
	mIsFixed = true;
	mFixedValue = value;
}

int Random::fixedValue() const
{
	return mFixedValue;
}

int Random::callsCount() const
{
	return mCallsCount;
}

int Random::lastValue() const
{
	return mLastValue;
}
