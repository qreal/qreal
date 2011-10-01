#include "sensorsConfigurer.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

SensorsConfigurer::SensorsConfigurer()
	: mLocked(true)
	, mSensorsToConfigure(0)
{
	mConfiguredSensors.resize(4);
	mPendingSensors.resize(4);
}

SensorsConfigurer::~SensorsConfigurer()
{
	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mPendingSensors)
		if (sensor != NULL && sensor != mConfiguredSensors[sensor->port()])
			delete sensor;

	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mConfiguredSensors)
		delete sensor;
}

void SensorsConfigurer::configureSensor(sensorImplementations::AbstractSensorImplementation *sensor, inputPort::InputPortEnum const &port)
{
	if (sensor != NULL && sensor->type() == mConfiguredSensors[sensor->port()]->type()) {
		// Then there is no need to configure it.
		return;
	}

	if (mConfiguredSensors[port] != mPendingSensors[port])
		delete mPendingSensors[port];
	mPendingSensors[port] = sensor;
	reconfigureSensors();
}

void SensorsConfigurer::lockConfiguring()
{
	mLocked = true;
}

void SensorsConfigurer::unlockConfiguring()
{
	mLocked = false;
	reconfigureSensors();
}

sensorImplementations::AbstractSensorImplementation * SensorsConfigurer::sensor(inputPort::InputPortEnum const &port) const
{
	return mConfiguredSensors[port];
}

void SensorsConfigurer::reconfigureSensors()
{
	if (mLocked)
		return;

	mSensorsToConfigure = 0;
	bool needToConfigure = false;
	for (inputPort::InputPortEnum i = inputPort::port1; i <= inputPort::port4; i = static_cast<inputPort::InputPortEnum>(i + 1))
	{
		if (mPendingSensors[i] == NULL) {
			delete mConfiguredSensors[i];
			mConfiguredSensors[i] = NULL;
		} else if (mConfiguredSensors[i] == NULL) {
			mConfiguredSensors[i] = mPendingSensors[i];
			connect(mConfiguredSensors[i], SIGNAL(configured()), this, SLOT(sensorConfiguredSlot()));
			++mSensorsToConfigure;
			needToConfigure = true;
			mConfiguredSensors[i]->configure();
		} else if (mPendingSensors[i]->type() != mConfiguredSensors[i]->type()) {
			delete mConfiguredSensors[i];
			mConfiguredSensors[i] = mPendingSensors[i];
			connect(mConfiguredSensors[i], SIGNAL(configured()), this, SLOT(sensorConfiguredSlot()));
			++mSensorsToConfigure;
			needToConfigure = true;
			mConfiguredSensors[i]->configure();
		}
	}
	if (!needToConfigure)
		emit allSensorsConfigured();
}

void SensorsConfigurer::sensorConfiguredSlot()
{
	sensorImplementations::AbstractSensorImplementation *sensor = dynamic_cast<sensorImplementations::AbstractSensorImplementation *>(sender());
	emit sensorConfigured(sensor);
	--mSensorsToConfigure;
	if (mSensorsToConfigure == 0)
		emit allSensorsConfigured();
}
