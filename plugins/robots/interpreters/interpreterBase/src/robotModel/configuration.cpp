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

void Configuration::configureDevice(robotParts::PluggableDevice const &device, PortInfo const &port)
{
	// TODO: Implement.
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
	// TODO: implement getting device by port direction
	return mConfiguredDevices.value(port, nullptr);
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
}

void Configuration::deviceConfiguredSlot(PortInfo const &port)
{
	robotParts::PluggableDevice *device = dynamic_cast<robotParts::PluggableDevice *>(sender());
	if (!device) {
		// TODO: Implement more adequate assertions mechanism.
		throw "Incorrect device configuration";
	}


}

void Configuration::reconfigureDevices()
{
}
