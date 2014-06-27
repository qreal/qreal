#include "modelTimer.h"

using namespace twoDModel;

ModelTimer::ModelTimer(Timeline const *timeline)
	: mTimeline(timeline), mTimeToWait(0)
	, mListening(false), mTimePast(0)
{
	connect(timeline, SIGNAL(tick()), this, SLOT(onTick()));
}

void ModelTimer::start(int ms)
{
	mTimeToWait = ms;
	mTimePast = 0;
	mListening = true;
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
