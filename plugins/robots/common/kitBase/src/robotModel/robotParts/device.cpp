#include "kitBase/robotModel/robotParts/device.h"

using namespace kitBase::robotModel;
using namespace kitBase::robotModel::robotParts;

/// Timeout (in milliseconds) for sensor configuration.
static const int configurationTimeout = 5000;

Device::Device(const DeviceInfo &info, const PortInfo &port)
	: mInfo(info)
	, mPort(port)
{
	mConfigurationTimeoutTimer.setSingleShot(true);
	mConfigurationTimeoutTimer.setInterval(configurationTimeout);
	connect(&mConfigurationTimeoutTimer, &QTimer::timeout, this, &Device::configurationTimerTimeoutSlot);
}

const PortInfo &Device::port() const
{
	return mPort;
}

const DeviceInfo &Device::deviceInfo() const
{
	return mInfo;
}

bool Device::ready() const
{
	/// @todo Implement.
	return true;
}

void Device::configure()
{
	mConfigurationTimeoutTimer.start();
	doConfiguration();
}

void Device::doConfiguration()
{
	configurationCompleted(true);
}

void Device::configurationCompleted(bool success)
{
	mConfigurationTimeoutTimer.stop();
	emit configured(success);
}

void Device::configurationTimerTimeoutSlot()
{
	emit configured(false);
}
