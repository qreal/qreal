#include "interpreterBase/robotModel/configuration.h"

using namespace interpreterBase::robotModel;

Configuration::Configuration()
	: mLocked(true)
{
}

Configuration::~Configuration()
{
	qDeleteAll(mPendingDevices);
	qDeleteAll(mConfiguredDevices);
}

void Configuration::configureDevice(robotParts::PluggableDevice * const device)
{
	Q_ASSERT(device);

	if (mConfiguredDevices.contains(device->port())
			&& mConfiguredDevices.value(device->port())->deviceInfo() == device->deviceInfo())
	{
		// It is same device that is already configured on that port, we need to do nothing.
		return;
	}

	delete mConfiguredDevices.value(device->port());
	mConfiguredDevices.remove(device->port());

	if (mPendingDevices.contains(device->port())) {
		if (mPendingDevices.value(device->port())->deviceInfo() == device->deviceInfo()) {
			// It is same device that is already pending for configuration on that port, we need to do nothing.
			return;
		}

		// QObject shall automatically disconnect on deletion, so we just forget about device not finished configuring.
		delete mPendingDevices.value(device->port());
		mPendingDevices.remove(device->port());
	}

	mPendingDevices.insert(device->port(), device);
	mConfigurationInProgress.remove(device->port());

	reconfigureDevices();
}

void Configuration::lockConfiguring()
{
	mLocked = true;
}

void Configuration::unlockConfiguring()
{
	mLocked = false;
	reconfigureDevices();
}

robotParts::PluggableDevice *Configuration::pluggableDevice(
		PortInfo const &port
		, PortDirection direction) const
{
	Q_UNUSED(direction);

	// TODO: implement getting device by port direction
	return mConfiguredDevices.value(port, nullptr);
}

QList<robotParts::PluggableDevice *> Configuration::pluggableDevices(PortDirection direction) const
{
	Q_UNUSED(direction);

	// TODO: implement port direction
	return mConfiguredDevices.values();
}

void Configuration::clearDevice(PortInfo const &port)
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

void Configuration::deviceConfiguredSlot(bool success)
{
	robotParts::PluggableDevice *device = dynamic_cast<robotParts::PluggableDevice *>(sender());
	if (!device) {
		// TODO: Implement more adequate assertions mechanism.
		throw "Incorrect device configuration";
	}

	if (mPendingDevices.value(device->port()) == device) {
		mPendingDevices.remove(device->port());
		mConfigurationInProgress.remove(device->port());
	} else {
		throw "mPendingDevices became corrupted during device initialization";
	}

	mConfiguredDevices.insert(device->port(), device);
}

void Configuration::reconfigureDevices()
{
	if (mLocked) {
		return;
	}

	for (robotParts::PluggableDevice * const device : mPendingDevices.values()) {
		if (!mConfigurationInProgress.contains(device->port())) {
			connect(device, &robotParts::PluggableDevice::configured, this, &Configuration::deviceConfiguredSlot);
			mConfigurationInProgress.insert(device->port());
			device->configure();
		}
	}
}
