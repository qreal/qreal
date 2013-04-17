#include "abstractTimer.h"

using namespace qReal::interpreters::robots::details;

void AbstractTimer::onTimeout()
{
	emit timeout();
}
