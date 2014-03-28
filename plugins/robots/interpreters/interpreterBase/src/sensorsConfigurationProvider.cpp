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
	if (!otherProvider) {
		return;
	}

	if (!mConnectedProviders.contains(otherProvider)) {
		mConnectedProviders << otherProvider;
		otherProvider->connectSensorsConfigurationProvider(this);
	}
}

void SensorsConfigurationProvider::sensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	if (mCurrentConfiguration[robotModel][port] != device) {
		mCurrentConfiguration[robotModel][port] = device;

		for (SensorsConfigurationProvider * const provider : mConnectedProviders) {
			// Broadcast change.
			provider->sensorConfigurationChanged(robotModel, port, device);

			// Allow connected providers to react on configuration change.
			provider->onSensorConfigurationChanged(robotModel, port, device);
		}
	}
}

void SensorsConfigurationProvider::onSensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port);
	Q_UNUSED(sensor);
}

void SensorsConfigurationProvider::nullifyConfiguration()
{
	for (QString const &robotModel : mCurrentConfiguration.keys()) {
		for (PortInfo const &port : mCurrentConfiguration[robotModel].keys()) {
			sensorConfigurationChanged(robotModel, port, DeviceInfo());
		}
	}
}

QStringList SensorsConfigurationProvider::configuredModels() const
{
	return mCurrentConfiguration.keys();
}

QList<robotModel::PortInfo> SensorsConfigurationProvider::configuredPorts(QString const &modelName) const
{
	if (!mCurrentConfiguration.contains(modelName)) {
		return {};
	}

	return mCurrentConfiguration[modelName].keys();
}

robotModel::DeviceInfo SensorsConfigurationProvider::currentConfiguration(QString const &modelName
		, robotModel::PortInfo const &port) const
{
	if (!mCurrentConfiguration.contains(modelName) || !mCurrentConfiguration[modelName].contains(port)) {
		return robotModel::DeviceInfo();
	}

	return mCurrentConfiguration[modelName][port];
}
