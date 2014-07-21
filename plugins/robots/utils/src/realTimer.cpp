#include <utils/realTimer.h>

using namespace utils;

RealTimer::RealTimer()
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void RealTimer::start(int ms)
{
	mTimer.setInterval(ms);
	mTimer.setSingleShot(true);
	mTimer.start();
}

void RealTimer::stop()
{
	mTimer.stop();
}
