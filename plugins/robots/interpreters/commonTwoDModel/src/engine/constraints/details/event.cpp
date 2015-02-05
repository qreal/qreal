#include "event.h"

using namespace twoDModel::constraints::details;

Event::Event(const Condition &condition, const Trigger &trigger, bool dropsOnFire)
	: mCondition(condition)
	, mTrigger(trigger)
	, mDropsOnFire(dropsOnFire)
{
}

bool Event::isAlive() const
{
	return mIsAlive;
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
