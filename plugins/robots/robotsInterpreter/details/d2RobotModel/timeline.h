#pragma once

#include <QtCore/QTimer>

#include "details/timelineInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/// A timeline returning 2D-model time in ms
class Timeline : public QObject, public TimelineInterface
{
	Q_OBJECT

public:
	static const int timeInterval = 10; // one cycle length
	static const int fps = 28; // frames per second
	static const int frameLength = 1000 / fps;

	static const int slowSpeedFactor = 2;
	static const int normalSpeedFactor = 5;
	static const int fastSpeedFactor = 10;

	explicit Timeline(QObject *parent = 0);

	int speedFactor() const;

	quint64 timestamp() const override;

	AbstractTimer *produceTimer() override;

public slots:
	void start();
	// Speed factor is also cycles per frame count
	void setSpeedFactor(int factor);

signals:
	void tick();
	void nextFrame();

private slots:
	void onTimer();
	void gotoNextFrame();

private:
	static const int realTimeInterval = 6;
	static const int ticksPerCycle = 3;

	QTimer mTimer;
	int mSpeedFactor;
	int mCyclesCount;
	qint64 mFrameStartTimestamp;
	bool mIsStarted;
	quint64 mTimestamp;
};

}
}
}
}
}
