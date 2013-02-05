#include "realTimer.h"

using namespace qReal::interpreters::robots::details;

void NullTimer::start(int ms)
{
	Q_UNUSED(ms)
}

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
