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
	static const int timeInterval = 2; // one cicle length
	static const int fps = 25; // frames per second
	static const int frameLength = 1000 / fps;

	explicit Timeline(QObject *parent = 0);

	int speedFactor() const;

public slots:
	void start();
	// Speed factor is also cicles per frame count
	void setSpeedFactor(int factor);

signals:
	void tick();
	void nextFrame();

private slots:
	void onTimer();
	void onFrameFinished();

private:
	QTimer mTimer;
	int mSpeedFactor;
	int mCiclesCount;
	qint64 mFrameStartTimestamp;
};

}
}
}
}
}
