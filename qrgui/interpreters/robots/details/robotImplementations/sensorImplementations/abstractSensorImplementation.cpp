#include "abstractSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

AbstractSensorImplementation::AbstractSensorImplementation(inputPort::InputPortEnum const &port)
	: mPort(port), mState(idle)
{
}

void AbstractSensorImplementation::setPort(inputPort::InputPortEnum const &port)
{
	mPort = port;
}
