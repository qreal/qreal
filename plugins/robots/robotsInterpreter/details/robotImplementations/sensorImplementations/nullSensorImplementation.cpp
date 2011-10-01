#include "nullSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullSensorImplementation::NullSensorImplementation(inputPort::InputPortEnum const &port, sensorType::SensorTypeEnum const &sensorType)
	: AbstractSensorImplementation(port, sensorType)
{
	mActiveWaitingTimer.setInterval(100);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void NullSensorImplementation::read()
{
	mActiveWaitingTimer.start();
}

void NullSensorImplementation::configure()
{
	emit configured();
}
