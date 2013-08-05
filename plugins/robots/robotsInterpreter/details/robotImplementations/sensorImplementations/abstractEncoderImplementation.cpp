#include "abstractEncoderImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

AbstractEncoderImplementation::AbstractEncoderImplementation(const enums::outputPort::OutputPortEnum &port)
	: mPort(port), mState(idle)
{
}

void AbstractEncoderImplementation::setPort(enums::outputPort::OutputPortEnum const &port)
{
	mPort = port;
}

void AbstractEncoderImplementation::sensorSpecificProcessResponse(const QByteArray &reading)
{
	Q_UNUSED(reading);
}
