#include "interpreterBase/robotModel/configuration.h"
#include "interpreterBase/robotModel/robotModelUtils.h"

using namespace interpreterBase::robotModel;

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
	qDeleteAll(mPendingDevices[input]);
	qDeleteAll(mPendingDevices[output]);
	qDeleteAll(mConfiguredDevices[input]);
	qDeleteAll(mConfiguredDevices[output]);
}

void Configuration::configureDevice(robotParts::Device * const device)
{
	Q_ASSERT(device);

	PortDirection const direction = RobotModelUtils::convertDirection(device->deviceInfo().direction());

	if (mConfiguredDevices.contains(direction) && mConfiguredDevices[direction].contains(device->port())
			&& mConfiguredDevices[direction].value(device->port())->deviceInfo() == device->deviceInfo())
	{
		// It is same device that is already configured on that port, we don't need to do anything.
		return;
	}

	delete mConfiguredDevices[direction].value(device->port());
	mConfiguredDevices[direction].remove(device->port());

	if (mConfiguredDevices.contains(direction) && mPendingDevices[direction].contains(device->port())) {
		if (mPendingDevices[direction].value(device->port())->deviceInfo() == device->deviceInfo()) {
			// It is same device that is already pending for configuration on that port, we don't need to do anything.
			return;
		}

		// QObject shall automatically disconnect on deletion, so we just forget about device not finished configuring.
		// It is not thread-safe, of course, so Configuration shall always run in one thread.
		delete mPendingDevices[direction].value(device->port());
		mPendingDevices[direction].remove(device->port());
	}

	mPendingDevices[direction].insert(device->port(), device);
	mConfigurationInProgress.remove(qMakePair(device->port(), direction));
}

robotParts::Device *Configuration::device(
		PortInfo const &port
		, PortDirection direction) const
{
	if (direction == defaultDirection) {
		direction = mConfiguredDevices[input].contains(port) ? input : output;
	}

	return mConfiguredDevices[direction].value(port, nullptr);
}

QList<robotParts::Device *> Configuration::devices(PortDirection direction) const
{
	return direction == defaultDirection
			? mConfiguredDevices[direction].values()
			: mConfiguredDevices[input].values() + mConfiguredDevices[output].values();
}

void Configuration::clearDevice(PortInfo const &port)
{
	for (PortDirection const direction : {input, output}) {
		if (mConfiguredDevices.contains(direction) && mConfiguredDevices[direction].contains(port)) {
			delete mConfiguredDevices[direction].value(port);
			mConfiguredDevices[direction].remove(port);
		}

		if (mPendingDevices.contains(direction) && mPendingDevices[direction].contains(port)) {
			// QObject shall automatically disconnect on deletion, so it is safe to just delete it now.
			delete mPendingDevices[direction].value(port);
			mPendingDevices[direction].remove(port);
		}

		mConfigurationInProgress.remove(qMakePair(port, direction));
	}
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

	PortDirection const direction = RobotModelUtils::convertDirection(device->deviceInfo().direction());
	if (mPendingDevices[direction].value(device->port()) == device) {
		mPendingDevices[direction].remove(device->port());
		mConfigurationInProgress.remove(qMakePair(device->port(), direction));
	} else {
		throw "mPendingDevices became corrupted during device initialization";
	}

	mConfiguredDevices[direction].insert(device->port(), device);

	checkAllDevicesConfigured();
}

void Configuration::applyConfiguration()
{
	checkAllDevicesConfigured();

	for (PortDirection const direction : mPendingDevices.keys()) {
		for (robotParts::Device * const device : mPendingDevices[direction].values()) {
			if (!mConfigurationInProgress.contains(qMakePair(device->port(), direction))) {
				mConfigurationInProgress.insert(qMakePair(device->port(), direction));
				connect(device, &robotParts::Device::configured, this, &Configuration::onDeviceConfigured);
				device->configure();
			}
		}
	}
}

void Configuration::checkAllDevicesConfigured()
{
	if (mPendingDevices[input].isEmpty() && mPendingDevices[output].isEmpty()) {
		emit allDevicesConfigured();
	}
}
