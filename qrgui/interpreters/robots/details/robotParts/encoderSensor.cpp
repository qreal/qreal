#include "encoderSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

EncoderSensor::EncoderSensor(robotImplementations::sensorImplementations::AbstractEncoderImplementation *encoderImpl, outputPort::OutputPortEnum const &port)
	: mPort(port), mEncoderImpl(encoderImpl)
{
}

EncoderSensor::~EncoderSensor()
{
}

void EncoderSensor::read()
{
	mEncoderImpl->read();
}

void EncoderSensor::nullificate()
{
	mEncoderImpl->nullificate();
}

details::robotImplementations::sensorImplementations::AbstractEncoderImplementation *EncoderSensor::encoderImpl()
{
	return mEncoderImpl;
}

void EncoderSensor::setImplementation(robotImplementations::sensorImplementations::AbstractEncoderImplementation *encoderImpl)
{
	mEncoderImpl = encoderImpl;
	mEncoderImpl->setPort(mPort);
}

