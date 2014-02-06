#include "interpreterBase/sensorsConfigurationProvider.h"

using namespace interpreterBase;
using namespace robotModel;

SensorsConfigurationProvider::SensorsConfigurationProvider(QString const &name)
	: mName(name)
{
}

SensorsConfigurationProvider::~SensorsConfigurationProvider()
{
}

void SensorsConfigurationProvider::connectSensorsConfigurationProvider(
		SensorsConfigurationProvider * const otherProvider)
{
	if (!mConnectedProviders.contains(otherProvider)) {
		mConnectedProviders << otherProvider;
		otherProvider->connectSensorsConfigurationProvider(this);
	}
}

void SensorsConfigurationProvider::sensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &sensor)
{
	if (mCurrentConfiguration[robotModel][port] != sensor) {
		mCurrentConfiguration[robotModel][port] = sensor;

		for (SensorsConfigurationProvider * const provider : mConnectedProviders) {
			// Allow connected providers to react on configuration change.
			provider->onSensorConfigurationChanged(robotModel, port, sensor);

			// Broadcast change.
			provider->sensorConfigurationChanged(robotModel, port, sensor);
		}
	}
}

void SensorsConfigurationProvider::refreshSensorsConfiguration()
{
	for (QString const &robotModel : mCurrentConfiguration.keys()) {
		for (PortInfo const &port : mCurrentConfiguration[robotModel].keys()) {
			onSensorConfigurationChanged(robotModel, port, mCurrentConfiguration[robotModel][port]);
		}
	}
}

void SensorsConfigurationProvider::onSensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port);
	Q_UNUSED(sensor);
}
