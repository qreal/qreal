#include "abstractSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

AbstractSensorImplementation::AbstractSensorImplementation(inputPort::InputPortEnum const &port
		, sensorType::SensorTypeEnum const &type)
	: mPort(port)
	, mState(idle)
	, mType(type)
{
}

void AbstractSensorImplementation::setPort(inputPort::InputPortEnum const &port)
{
	mPort = port;
}

inputPort::InputPortEnum AbstractSensorImplementation::port() const
{
	return mPort;
}

sensorType::SensorTypeEnum AbstractSensorImplementation::type() const
{
	return mType;
}
