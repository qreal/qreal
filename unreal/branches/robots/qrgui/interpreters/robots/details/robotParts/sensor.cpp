#include "sensor.h"
#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;
using namespace robotParts;

Sensor::Sensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port)
	: mPort(port), mSensorImpl(sensorImpl)
{
	mSensorImpl->setPort(mPort);
}

Sensor::~Sensor()
{
	delete mSensorImpl;
}

void Sensor::read()
{
	mSensorImpl->read();
}

AbstractSensorImplementation *Sensor::sensorImpl()
{
	return mSensorImpl;
}
