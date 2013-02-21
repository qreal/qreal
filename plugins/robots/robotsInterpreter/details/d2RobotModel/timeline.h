#pragma once

#include <QtCore/QTimer>

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace d2Model
{

class Timeline : public QObject
{
	Q_OBJECT

public:
	static const int timeInterval = 2; // one cycle length
	static const int fps = 28; // frames per second
	static const int frameLength = 1000 / fps;

	explicit Timeline(QObject *parent = 0);

	int speedFactor() const;

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
};

}
}
}
}
}
