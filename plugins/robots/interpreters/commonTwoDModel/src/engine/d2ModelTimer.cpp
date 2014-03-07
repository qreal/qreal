#include "d2ModelTimer.h"

using namespace twoDModel;

D2ModelTimer::D2ModelTimer(Timeline const *timeline)
	: mTimeline(timeline), mTimeToWait(0)
	, mListening(false), mTimePast(0)
{
	connect(timeline, SIGNAL(tick()), this, SLOT(onTick()));
}

void D2ModelTimer::start(int ms)
{
	mTimeToWait = ms;
	mTimePast = 0;
	mListening = true;
}

void D2ModelTimer::onTick()
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
