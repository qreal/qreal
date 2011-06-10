#include "sensorsConfiguration.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SensorsConfiguration::SensorsConfiguration()
{
	mSensors.resize(4);
}

void SensorsConfiguration::setSensor(inputPort::InputPortEnum const &port
		, sensorType::SensorTypeEnum const &type
		, QPoint const &position
		, qreal const &direction)
{
	mSensors[port] = SensorInfo(position, direction, type);
}

void SensorsConfiguration::setPosition(inputPort::InputPortEnum const &port, QPoint const &position)
{
	mSensors[port].setPosition(position);
}

QPoint SensorsConfiguration::position(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].position();
}

void SensorsConfiguration::setDirection(inputPort::InputPortEnum const &port, qreal direction)
{
	mSensors[port].setDirection(direction);
}

qreal SensorsConfiguration::direction(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].direction();
}

sensorType::SensorTypeEnum SensorsConfiguration::type(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].type();
}

void SensorsConfiguration::clearSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = SensorInfo();
}



SensorsConfiguration::SensorInfo::SensorInfo()
	: mDirection(0), mSensorType(sensorType::unused)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPoint const &position
		, qreal direction
		, sensorType::SensorTypeEnum const &sensorType
)
	: mPosition(position), mDirection(direction), mSensorType(sensorType)
{
}

QPoint SensorsConfiguration::SensorInfo::position() const
{
	return mPosition;
}

void SensorsConfiguration::SensorInfo::setPosition(QPoint const &position)
{
	mPosition = position;
}

qreal SensorsConfiguration::SensorInfo::direction() const
{
	return mDirection;
}

void SensorsConfiguration::SensorInfo::setDirection(qreal direction)
{
	mDirection = direction;
}

sensorType::SensorTypeEnum SensorsConfiguration::SensorInfo::type() const
{
	return mSensorType;
}
