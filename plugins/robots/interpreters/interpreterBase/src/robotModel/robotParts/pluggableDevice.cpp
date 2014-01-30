#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"

using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

/// Timeout (in milliseconds) for sensor configuration.
static int const configurationTimeout = 5000;

PluggableDevice::PluggableDevice(PluggableDeviceInfo const &info, PortInfo const &port)
	: mInfo(info)
	, mPort(port)
{
	mConfigurationTimeoutTimer.setSingleShot(true);
	mConfigurationTimeoutTimer.setInterval(configurationTimeout);
	connect(&mConfigurationTimeoutTimer, &QTimer::timeout, this, &PluggableDevice::configurationTimerTimeoutSlot);
}

PluggableDevice::~PluggableDevice()
{
}

PortInfo const &PluggableDevice::port() const
{
	return mPort;
}

PluggableDeviceInfo const &PluggableDevice::deviceInfo() const
{
	return mInfo;
}

bool PluggableDevice::ready() const
{
	// TODO: Implement.
	return true;
}

void PluggableDevice::configure()
{
	mConfigurationTimeoutTimer.start();
	doConfiguration();
}

void PluggableDevice::configurationCompleted(bool success)
{
	mConfigurationTimeoutTimer.stop();
	emit configured(success);
}

void PluggableDevice::configurationTimerTimeoutSlot()
{
	emit configured(false);
}
