#include "sensorsConfigurationProvider.h"

using namespace qReal::interpreters::robots::details;

SensorsConfigurationProvider::SensorsConfigurationProvider(QString const &name)
	: mName(name)
{
	mCurrentConfiguration.resize(4);
	for (int i = 0; i < 4; ++i) {
		mCurrentConfiguration[i] = enums::sensorType::unused;
	}
}

SensorsConfigurationProvider::~SensorsConfigurationProvider()
{
}

void SensorsConfigurationProvider::connectSensorsConfigurationProvider(
		SensorsConfigurationProvider * const otherProvider
		)
{
	if (!mConnectedProviders.contains(otherProvider)) {
		mConnectedProviders << otherProvider;
		otherProvider->connectSensorsConfigurationProvider(this);
	}
}

void SensorsConfigurationProvider::sensorConfigurationChanged(
		enums::inputPort::InputPortEnum port
		, enums::sensorType::SensorTypeEnum type
		)
{
	if (mCurrentConfiguration[port] != type) {
		mCurrentConfiguration[port] = type;

		for (SensorsConfigurationProvider * const provider : mConnectedProviders) {
			// Allow connected providers to react on configuration change.
			provider->onSensorConfigurationChanged(port, type);

			// Broadcast change.
			provider->sensorConfigurationChanged(port, type);
		}
	}
}

void SensorsConfigurationProvider::refreshSensorsConfiguration()
{
	onSensorConfigurationChanged(enums::inputPort::port1, mCurrentConfiguration[enums::inputPort::port1]);
	onSensorConfigurationChanged(enums::inputPort::port2, mCurrentConfiguration[enums::inputPort::port2]);
	onSensorConfigurationChanged(enums::inputPort::port3, mCurrentConfiguration[enums::inputPort::port3]);
	onSensorConfigurationChanged(enums::inputPort::port4, mCurrentConfiguration[enums::inputPort::port4]);
}

void SensorsConfigurationProvider::onSensorConfigurationChanged(
		enums::inputPort::InputPortEnum port
		, enums::sensorType::SensorTypeEnum type
		)
{
	Q_UNUSED(port);
	Q_UNUSED(type);
}
