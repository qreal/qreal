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

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>

#include "twoDModel/engine/model/timeline.h"
#include "modelTimer.h"

using namespace twoDModel::model;

Timeline::Timeline(QObject *parent)
	: QObject(parent)
	, mSpeedFactor(normalSpeedFactor)
	, mCyclesCount(0)
	, mIsStarted(false)
	, mTimestamp(0)
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	mTimer.setInterval(defaultRealTimeInterval);
}

void Timeline::start()
{
	if (!mIsStarted) {
		mIsStarted = true;
		emit started();
		gotoNextFrame();
	}
}

void Timeline::stop(qReal::interpretation::StopReason reason)
{
	if (mIsStarted) {
		mIsStarted = false;
		QCoreApplication::processEvents();
		emit beforeStop(reason);
		mTimer.stop();
		emit stopped(reason);
	}
}

void Timeline::onTimer()
{
	if (!mIsStarted) {
		mTimer.stop();
		return;
	}

	for (int i = 0; i < ticksPerCycle; ++i) {
		QCoreApplication::processEvents();
		if (mIsStarted) {
			mTimestamp += timeInterval;
			emit tick();
			++mCyclesCount;
			if (mCyclesCount >= mSpeedFactor) {
				mTimer.stop();
				mCyclesCount = 0;
				const int msFromFrameStart = static_cast<int>(QDateTime::currentMSecsSinceEpoch()
						- mFrameStartTimestamp);
				const int pauseBeforeFrameEnd = mFrameLength - msFromFrameStart;
				if (pauseBeforeFrameEnd > 0) {
					QTimer::singleShot(pauseBeforeFrameEnd - 1, this, SLOT(gotoNextFrame()));
				} else {
					gotoNextFrame();
				}

				return;
			}
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

bool Timeline::isStarted() const
{
	return mIsStarted;
}

quint64 Timeline::timestamp() const
{
	return mTimestamp;
}

utils::AbstractTimer *Timeline::produceTimer()
{
	return new ModelTimer(this);
}

void Timeline::setImmediateMode(bool immediateMode)
{
	mTimer.setInterval(immediateMode ? 0 : defaultRealTimeInterval);
	setSpeedFactor(immediateMode ? immediateSpeedFactor : normalSpeedFactor);
	mFrameLength = immediateMode ? 0 : defaultFrameLength;
}

void Timeline::setSpeedFactor(int factor)
{
	if (mSpeedFactor != factor) {
		mSpeedFactor = factor;
		emit speedFactorChanged(factor);
	}
}
