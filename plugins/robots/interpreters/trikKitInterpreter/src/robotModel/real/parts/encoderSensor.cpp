#include "encoderSensor.h"

#include <utils/tracer.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::EncoderSensor(info, port)
{
}

void EncoderSensor::read()
{
	emit newData(0);
}

void EncoderSensor::nullify()
{
}

void EncoderSensor::doConfiguration()
{
	configurationCompleted(true);
}
