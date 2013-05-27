#include "realTimer.h"

using namespace qReal::interpreters::robots::details;

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