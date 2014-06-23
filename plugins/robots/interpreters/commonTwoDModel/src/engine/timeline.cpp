#include <QtCore/QDateTime>

#include "timeline.h"
#include "d2ModelTimer.h"

using namespace twoDModel;

Timeline::Timeline(QObject *parent)
	: QObject(parent)
	, mSpeedFactor(normalSpeedFactor)
	, mCyclesCount(0)
	, mIsStarted(false)
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	mTimer.setInterval(realTimeInterval);
}

void Timeline::start()
{
	if (!mIsStarted) {
		mIsStarted = true;
		gotoNextFrame();
	}
}

void Timeline::onTimer()
{
	for (int i = 0; i < ticksPerCycle; ++i) {
		mTimestamp += timeInterval;
		emit tick();
		++mCyclesCount;
		if (mCyclesCount >= mSpeedFactor) {
			mTimer.stop();
			mCyclesCount = 0;
			int const msFromFrameStart = static_cast<int>(QDateTime::currentMSecsSinceEpoch()
					- mFrameStartTimestamp);
			int const pauseBeforeFrameEnd = frameLength - msFromFrameStart;
			if (pauseBeforeFrameEnd > 0) {
				QTimer::singleShot(pauseBeforeFrameEnd - 1, this, SLOT(gotoNextFrame()));
			} else {
				gotoNextFrame();
			}
			return;
		}
	}
}

void Timeline::gotoNextFrame()
{
	emit nextFrame();
	mFrameStartTimestamp = QDateTime::currentMSecsSinceEpoch();
	if (!mTimer.isActive()) {
		mTimer.start();
	}
}

int Timeline::speedFactor() const
{
	return mSpeedFactor;
}

quint64 Timeline::timestamp() const
{
	return mTimestamp;
}

utils::AbstractTimer *Timeline::produceTimer()
{
	return new D2ModelTimer(this);
}

void Timeline::setSpeedFactor(int factor)
{
	mSpeedFactor = factor;
}
