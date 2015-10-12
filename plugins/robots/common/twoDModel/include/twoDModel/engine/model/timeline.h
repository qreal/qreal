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

#pragma once

#include <QtCore/QTimer>

#include <qrutils/interpreter/stopReason.h>
#include <utils/timelineInterface.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {
namespace model {

/// A timeline returning 2D-model time in ms
class TWO_D_MODEL_EXPORT Timeline : public QObject, public utils::TimelineInterface
{
	Q_OBJECT

public:
	static const int timeInterval = 10; // one cycle length
	static const int fps = 28; // frames per second
	static const int defaultFrameLength = 1000 / fps;

	static const int normalSpeedFactor = 5;
	static const int immediateSpeedFactor = 100000000;

	explicit Timeline(QObject *parent = 0);

	int speedFactor() const;

	/// Returns true if timeline is ticking at the moment.
	bool isStarted() const;

	quint64 timestamp() const override;

	utils::AbstractTimer *produceTimer() override;

	/// If @arg immediateMode is true then timeline will emit ticks without delay.
	/// Thus the immediate process modeling may be performed in background.
	void setImmediateMode(bool immediateMode);

public slots:
	void start();
	void stop(qReal::interpretation::StopReason reason);

	// Speed factor is also cycles per frame count
	void setSpeedFactor(int factor);

signals:
	void tick();
	void nextFrame();

	/// Emitted just before timeline will emit its first tick.
	/// @param reason The reason why the interpretation stopped.
	void started();

	/// Emitted when timeline stop was requested, just before internal timers will stop.
	/// @param reason The reason why the interpretation stopped.
	void beforeStop(qReal::interpretation::StopReason reason);

	/// Emitted just after timeline has stopped its ticking.
	void stopped(qReal::interpretation::StopReason reason);

	/// Emitted when timeline speed factor value changes.
	void speedFactorChanged(int value);

private slots:
	void onTimer();
	void gotoNextFrame();

private:
	static const int defaultRealTimeInterval = 0;
	static const int ticksPerCycle = 3;

	QTimer mTimer;
	int mSpeedFactor;
	int mCyclesCount;
	qint64 mFrameStartTimestamp;
	bool mIsStarted;
	quint64 mTimestamp;
	int mFrameLength = defaultFrameLength;
};

}
}
