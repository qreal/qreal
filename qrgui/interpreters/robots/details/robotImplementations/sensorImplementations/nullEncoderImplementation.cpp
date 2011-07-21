#include "nullEncoderImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullEncoderImplementation::NullEncoderImplementation(const outputPort::OutputPortEnum &port)
	: AbstractEncoderImplementation(port)
{
}

void NullEncoderImplementation::timerTimeout()
{
	emit response(0);
}

void NullEncoderImplementation::read()
{
	mActiveWaitingTimer.start();
}

