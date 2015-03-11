#pragma once

#include <QtCore/QTimer>

#include "utils/timelineInterface.h"

namespace twoDModel {
namespace model {

/// A timeline returning 2D-model time in ms
class Timeline : public QObject, public utils::TimelineInterface
{
	Q_OBJECT

public:
	static const int timeInterval = 10; // one cycle length
	static const int fps = 28; // frames per second
	static const int defaultFrameLength = 1000 / fps;

	static const int slowSpeedFactor = 2;
	static const int normalSpeedFactor = 5;
	static const int fastSpeedFactor = 10;
	static const int immediateSpeedFactor = 100000000;

	explicit Timeline(QObject *parent = 0);

	int speedFactor() const;

	quint64 timestamp() const override;

	utils::AbstractTimer *produceTimer() override;

	/// If @arg immediateMode is true then timeline will emit ticks without delay.
	/// Thus the immediate process modeling may be performed in background.
	void setImmediateMode(bool immediateMode);

public slots:
	void start();
	void stop();

	// Speed factor is also cycles per frame count
	void setSpeedFactor(int factor);

signals:
	void tick();
	void nextFrame();

	void started();
	void stopped();

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
