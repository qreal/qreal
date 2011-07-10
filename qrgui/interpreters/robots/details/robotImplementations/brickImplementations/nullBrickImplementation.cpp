#include "nullBrickImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::brickImplementations;

NullBrickImplementation::NullBrickImplementation()
{
}

void NullBrickImplementation::playTone(unsigned freq, unsigned time)
{
	Q_UNUSED(freq);
	Q_UNUSED(time);
}

void NullBrickImplementation::beep(unsigned time)
{
	Q_UNUSED(time);
}
