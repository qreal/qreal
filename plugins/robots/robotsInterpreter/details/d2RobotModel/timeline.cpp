#include <QtCore/QDateTime>

#include "timeline.h"

using namespace qReal::interpreters::robots::details::d2Model;

Timeline::Timeline(QObject *parent)
	: QObject(parent)
	, mSpeedFactor(5) // Normal speed
	, mCyclesCount(0)
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	mTimer.setInterval(realTimeInterval);
}

void Timeline::start()
{
	gotoNextFrame();
}

void Timeline::onTimer()
{
	for (int i = 0; i < ticksPerCycle; ++i) {
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

void Timeline::setSpeedFactor(int factor)
{
//	gotoNextFrame();
	mSpeedFactor = factor;
}
