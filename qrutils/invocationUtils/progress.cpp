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

#include "progress.h"

using namespace invocation;

Progress::Progress(int minimum, int maximum)
	: mMinimum(INT_MIN), mMaximum(INT_MAX) // for setMin/Max function worked correctly
{
	setMinimum(minimum);
	setMaximum(maximum);
	setValue(minimum);
}

int Progress::value() const
{
	return mValue;
}

int Progress::minimum() const
{
	return mMinimum;
}

int Progress::maximum() const
{
	return mMaximum;
}

void Progress::setValue(int value)
{
	if (value < mMinimum || value > mMaximum) {
		return;
	}
	const bool changed = value != mValue;
	mValue = value;
	if (changed) {
		emit valueChanged(value);
	}
}

void Progress::setMinimum(int minimum)
{
	if (minimum > maximum()) {
		return;
	}
	const bool changed = minimum != mMinimum;
	mMinimum = minimum;
	if (changed) {
		emit minimumChanged(minimum);
	}
}

void Progress::setMaximum(int maximum)
{
	if (maximum < minimum()) {
		return;
	}
	const bool changed = maximum != mMaximum;
	mMaximum = maximum;
	if (changed) {
		emit maximumChanged(maximum);
	}
}
