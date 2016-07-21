/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "kitBase/devicesConfigurationProvider.h"

using namespace kitBase;
using namespace robotModel;

DevicesConfigurationProvider::DevicesConfigurationProvider(const QString &name)
	: mName(name)
{
}

DevicesConfigurationProvider::~DevicesConfigurationProvider()
{
}

void DevicesConfigurationProvider::connectDevicesConfigurationProvider(
		DevicesConfigurationProvider * const otherProvider)
{
	if (!otherProvider) {
		return;
	}

	if (!mConnectedProviders.contains(otherProvider)) {
		mConnectedProviders << otherProvider;
		otherProvider->connectDevicesConfigurationProvider(this);
	}
}

void DevicesConfigurationProvider::disconnectDevicesConfigurationProvider(
		DevicesConfigurationProvider * const provider)
{
	if (!provider) {
		return;
	}

	mConnectedProviders.removeOne(provider);
}

void DevicesConfigurationProvider::disconnectDevicesConfigurationProvider()
{
	for (DevicesConfigurationProvider *provider : mConnectedProviders) {
		provider->disconnectDevicesConfigurationProvider(this);
	}
}

void DevicesConfigurationProvider::deviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &device, Reason reason)
{
	if (mCurrentConfiguration[robotModel][port] != device) {
		mCurrentConfiguration[robotModel][port] = device;

		for (DevicesConfigurationProvider * const provider : mConnectedProviders) {
			// Broadcast change.
			provider->deviceConfigurationChanged(robotModel, port, device, reason);

			// Allow connected providers to react on configuration change.
			provider->onDeviceConfigurationChanged(robotModel, port, device, reason);
		}
	}
}

void DevicesConfigurationProvider::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &sensor, Reason reason)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port);
	Q_UNUSED(sensor);
	Q_UNUSED(reason);
}

void DevicesConfigurationProvider::clearConfiguration(Reason reason)
{
	for (const QString &robotModel : mCurrentConfiguration.keys()) {
		for (const PortInfo &port : mCurrentConfiguration[robotModel].keys()) {
			deviceConfigurationChanged(robotModel, port, DeviceInfo(), reason);
		}
	}
}

QStringList DevicesConfigurationProvider::configuredModels() const
{
	return mCurrentConfiguration.keys();
}

QList<robotModel::PortInfo> DevicesConfigurationProvider::configuredPorts(const QString &modelName) const
{
	if (!mCurrentConfiguration.contains(modelName)) {
		return {};
	}

	return mCurrentConfiguration[modelName].keys();
}

robotModel::DeviceInfo DevicesConfigurationProvider::currentConfiguration(const QString &modelName
		, const robotModel::PortInfo &port) const
{
	if (!mCurrentConfiguration.contains(modelName) || !mCurrentConfiguration[modelName].contains(port)) {
		return robotModel::DeviceInfo();
	}

	return mCurrentConfiguration[modelName][port];
}
