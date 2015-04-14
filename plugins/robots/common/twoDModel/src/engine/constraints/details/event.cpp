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

#include "event.h"

using namespace twoDModel::constraints::details;

Event::Event(const QString &id
		, const Condition &condition
		, const Trigger &trigger
		, bool dropsOnFire
		, bool isSettedInitially)
	: mId(id)
	, mIsAlive(isSettedInitially)
	, mCondition(condition)
	, mTrigger(trigger)
	, mDropsOnFire(dropsOnFire)
	, mIsSettedInitially(isSettedInitially)
{
}

QString Event::id() const
{
	return mId;
}

bool Event::isAlive() const
{
	return mIsAlive;
}

bool Event::isAliveInitially() const
{
	return mIsSettedInitially;
}

void Event::setUp()
{
	mIsAlive = true;
	emit settedUp();
}

void Event::drop()
{
	mIsAlive = false;
	emit dropped();
}

void Event::check()
{
	if (!mIsAlive || !mCondition()) {
		return;
	}

	emit fired();
	mTrigger();
	if (mDropsOnFire) {
		drop();
	}
}

void Event::setCondition(const Condition &condition)
{
	mCondition = condition;
}
