#include <QtCore/QDateTime>

#include "timeline.h"

using namespace qReal::interpreters::robots::details::d2Model;

Timeline::Timeline(QObject *parent)
	: QObject(parent)
	, mSpeedFactor(10) // Normal speed
	, mCiclesCount(0)
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));

	mTimer.setInterval(timeInterval);
}

void Timeline::start()
{
	onFrameFinished();
}

void Timeline::onTimer()
{
	emit tick();
	++mCiclesCount;
	if (mCiclesCount >= mSpeedFactor) {
		mTimer.stop();
		mCiclesCount = 0;
		int const msFromFrameStart = (int) (QDateTime::currentMSecsSinceEpoch()
				- mFrameStartTimestamp);
		int const pauseBeforeFrameEnd = frameLength - msFromFrameStart;
		if (pauseBeforeFrameEnd > 0) {
			QTimer::singleShot(pauseBeforeFrameEnd-1, this, SLOT(onFrameFinished()));
		} else {
			onFrameFinished();
		}
	}
}

void Timeline::onFrameFinished()
{
	emit nextFrame();
	mFrameStartTimestamp = QDateTime::currentMSecsSinceEpoch();
	mTimer.start();
}

int Timeline::speedFactor() const
{
	return mSpeedFactor;
}

void Timeline::setSpeedFactor(int factor)
{
	mSpeedFactor = factor;
}
