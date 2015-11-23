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

#include "kitBase/robotModel/configuration.h"

using namespace kitBase::robotModel;

Configuration::~Configuration()
{
	qDeleteAll(mPendingDevices);
	qDeleteAll(mConfiguredDevices);
}

void Configuration::configureDevice(robotParts::Device * const device)
{
	Q_ASSERT(device);

	if (mConfiguredDevices.contains(device->port())
			&& mConfiguredDevices.value(device->port())->deviceInfo() == device->deviceInfo()
			&& device->port().name() != "DisplayPort")  // hack for cleaning display in 2D model
	{
		// It is same device that is already configured on that port, we don't need to do anything.
		return;
	}

	delete mConfiguredDevices.value(device->port());
	mConfiguredDevices.remove(device->port());

	if (mPendingDevices.contains(device->port())) {
		if (mPendingDevices.value(device->port())->deviceInfo() == device->deviceInfo()
			&& device->port().name() != "DisplayPort") { // hack for cleaning display in 2D model
			// It is same device that is already pending for configuration on that port, we don't need to do anything.
			return;
		}

		// QObject shall automatically disconnect on deletion, so we just forget about device not finished configuring.
		// It is not thread-safe, of course, so Configuration shall always run in one thread.
		delete mPendingDevices.value(device->port());
		mPendingDevices.remove(device->port());
	}

	mPendingDevices.insert(device->port(), device);
	mConfigurationInProgress.remove(device->port());
}

robotParts::Device *Configuration::device(const PortInfo &port) const
{
	return mConfiguredDevices.value(port, nullptr);
}

QList<robotParts::Device *> Configuration::devices() const
{
	return mConfiguredDevices.values();
}

void Configuration::clearDevice(const PortInfo &port)
{
	if (mConfiguredDevices.contains(port)) {
		delete mConfiguredDevices.value(port);
		mConfiguredDevices.remove(port);
	}

	if (mPendingDevices.contains(port)) {
		// QObject shall automatically disconnect on deletion, so it is safe to just delete it now.
		delete mPendingDevices.value(port);
		mPendingDevices.remove(port);
	}

	mConfigurationInProgress.remove(port);
}

void Configuration::onDeviceConfigured(bool success)
{
	/// @todo Do something with failure of configuration.
	Q_UNUSED(success);

	robotParts::Device *device = dynamic_cast<robotParts::Device *>(sender());
	if (!device) {
		/// @todo Implement more adequate assertions mechanism.
		throw "Incorrect device configuration";
	}

	if (mPendingDevices.value(device->port()) == device) {
		mPendingDevices.remove(device->port());
		mConfigurationInProgress.remove(device->port());
	} else {
		throw "mPendingDevices became corrupted during device initialization";
	}

	mConfiguredDevices.insert(device->port(), device);

	emit deviceConfigured(device);
	checkAllDevicesConfigured();
}

void Configuration::applyConfiguration()
{
	checkAllDevicesConfigured();

	for (robotParts::Device * const device : mPendingDevices.values()) {
		if (!mConfigurationInProgress.contains(device->port())) {
			mConfigurationInProgress.insert(device->port());
			connect(device, &robotParts::Device::configured, this, &Configuration::onDeviceConfigured);
			device->configure();
		}
	}
}

void Configuration::checkAllDevicesConfigured()
{
	if (mPendingDevices.isEmpty()) {
		emit allDevicesConfigured();
	}
}
