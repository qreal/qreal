#include "interpreterBase/robotModel/configuration.h"

using namespace interpreterBase::robotModel;

Configuration::Configuration()
	: mLocked(true)
	, mWasConfigurationRequest(false)
{
}

Configuration::~Configuration()
{
	qDeleteAll(mPendingDevices);
	qDeleteAll(mConfiguredDevices);
}

void Configuration::configureDevice(robotParts::Device * const device)
{
	Q_ASSERT(device);

	mWasConfigurationRequest = true;

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

robotParts::Device *Configuration::device(
		PortInfo const &port
		, PortDirection direction) const
{
	Q_UNUSED(direction);

	/// @todo Implement getting device by port direction.
	return mConfiguredDevices.value(port, nullptr);
}

QList<robotParts::Device *> Configuration::devices(PortDirection direction) const
{
	Q_UNUSED(direction);

	/// @todo implement port direction
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

void Configuration::forceResponse()
{
	mWasConfigurationRequest = true;
	checkAllDevicesConfigured();
}

void Configuration::deviceConfiguredSlot(bool success)
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

	checkAllDevicesConfigured();
}

void Configuration::reconfigureDevices()
{
	if (mLocked) {
		return;
	}

	checkAllDevicesConfigured();

	for (robotParts::Device * const device : mPendingDevices.values()) {
		if (!mConfigurationInProgress.contains(device->port())) {
			connect(device, &robotParts::Device::configured, this, &Configuration::deviceConfiguredSlot);
			mConfigurationInProgress.insert(device->port());
			device->configure();
		}
	}
}

void Configuration::checkAllDevicesConfigured()
{
	if (!mLocked && mPendingDevices.isEmpty() && mConfigurationInProgress.isEmpty() && mWasConfigurationRequest) {
		mWasConfigurationRequest = false;
		emit allDevicesConfigured();
	}
}
