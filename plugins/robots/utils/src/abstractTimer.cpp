#include <utils/abstractTimer.h>

using namespace utils;

void AbstractTimer::onTimeout()
{
	emit timeout();
}
