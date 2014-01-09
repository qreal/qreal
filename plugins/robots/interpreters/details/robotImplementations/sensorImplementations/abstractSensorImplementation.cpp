#include "abstractSensorImplementation.h"

using namespace qReal::interpreters;
using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

AbstractSensorImplementation::AbstractSensorImplementation(robots::enums::inputPort::InputPortEnum const port
		, robots::enums::sensorType::SensorTypeEnum const &type)
	: mPort(port)
	, mState(idle)
	, mType(type)
{
}

void AbstractSensorImplementation::setPort(robots::enums::inputPort::InputPortEnum const port)
{
	mPort = port;
}

robots::enums::inputPort::InputPortEnum AbstractSensorImplementation::port() const
{
	return mPort;
}

robots::enums::sensorType::SensorTypeEnum AbstractSensorImplementation::type() const
{
	return mType;
}

void AbstractSensorImplementation::nullify()
{
	emit response(0);
}
