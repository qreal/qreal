#include "sensorsConfigurer.h"

#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace qReal::interpreters::robots::details;
using namespace qReal::interpreters::robots::details::robotImplementations;

SensorsConfigurer::SensorsConfigurer()
	: mLocked(true)
	, mSensorsToConfigure(0)
{
	mConfiguredSensors.resize(4);
	mPendingSensors.resize(4);
}

SensorsConfigurer::~SensorsConfigurer()
{
	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mPendingSensors) {
		if (sensor != NULL && sensor != mConfiguredSensors[sensor->port()]) {
			delete sensor;
		}
	}

	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mConfiguredSensors) {
		delete sensor;
	}
}

void SensorsConfigurer::configureSensor(sensorImplementations::AbstractSensorImplementation *sensor, inputPort::InputPortEnum const &port)
{
	Tracer::debug(tracer::initialization, "SensorsConfigurer::configureSensor", "Configuring sensor on port " + QString::number(port));
	if (sensor == NULL
		|| mConfiguredSensors[sensor->port()] == NULL
		|| sensor->type() != mConfiguredSensors[sensor->port()]->type())
	{
		if (mConfiguredSensors[port] != mPendingSensors[port]) {
			delete mPendingSensors[port];
		}
		mPendingSensors[port] = sensor;
	}
	reconfigureSensors();
}

void SensorsConfigurer::lockConfiguring()
{
	Tracer::debug(tracer::initialization, "SensorsConfigurer::lockConfiguring", "Sensor configuration locked, all configuration requests are queued.");
	mLocked = true;
}

void SensorsConfigurer::unlockConfiguring()
{
	Tracer::debug(tracer::initialization, "SensorsConfigurer::unlockConfiguring", "Sensor configuration unlocked, running queued configurations.");
	mLocked = false;
	reconfigureSensors();
}

sensorImplementations::AbstractSensorImplementation * SensorsConfigurer::sensor(inputPort::InputPortEnum const &port) const
{
	return mConfiguredSensors[port];
}

void SensorsConfigurer::reconfigureSensors()
{
	if (mLocked) {
		return;
	}

	Tracer::debug(tracer::initialization, "SensorsConfigurer::reconfigureSensors", "Going to reconfigure sensors");
	mSensorsToConfigure = 0;
	QList<int> sensorsToConfigure;
	for (inputPort::InputPortEnum i = inputPort::port1; i <= inputPort::port4; i = static_cast<inputPort::InputPortEnum>(i + 1))
	{
		if (mPendingSensors[i] == NULL) {
			delete mConfiguredSensors[i];
			mConfiguredSensors[i] = NULL;
		} else if (mConfiguredSensors[i] == NULL) {
			mConfiguredSensors[i] = mPendingSensors[i];
			connect(mConfiguredSensors[i], SIGNAL(configured()), this, SLOT(sensorConfiguredSlot()));
			++mSensorsToConfigure;
			sensorsToConfigure << i;
		} else if (mPendingSensors[i]->type() != mConfiguredSensors[i]->type()) {
			delete mConfiguredSensors[i];
			mConfiguredSensors[i] = mPendingSensors[i];
			connect(mConfiguredSensors[i], SIGNAL(configured()), this, SLOT(sensorConfiguredSlot()));
			++mSensorsToConfigure;
			sensorsToConfigure << i;
		}
	}
	if (sensorsToConfigure.isEmpty()) {
		Tracer::debug(tracer::initialization, "SensorsConfigurer::reconfigureSensors", "No need to wait for a response, sending allSensorsReconfigured signal now.");
		emit allSensorsConfigured();
	} else {
		foreach (int const &i, sensorsToConfigure) {
			mConfiguredSensors[i]->configure();
		}
	}
}

void SensorsConfigurer::sensorConfiguredSlot()
{
	--mSensorsToConfigure;
	Tracer::debug(tracer::initialization, "SensorsConfigurer::sensorConfiguredSlot", "Sensor configured, " + QString::number(mSensorsToConfigure) + " remaining");
	sensorImplementations::AbstractSensorImplementation *sensor = dynamic_cast<sensorImplementations::AbstractSensorImplementation *>(sender());
	emit sensorConfigured(sensor);
	if (mSensorsToConfigure == 0) {
		emit allSensorsConfigured();
	}
}
