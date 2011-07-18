#include "encoderSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

EncoderSensor::EncoderSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port)
	: Sensor(sensorImpl,port)
{
}
