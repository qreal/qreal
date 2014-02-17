#include "interpreterBase/robotModel/robotParts/device.h"

using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

/// Timeout (in milliseconds) for sensor configuration.
static int const configurationTimeout = 5000;

Device::Device(DeviceInfo const &info, PortInfo const &port)
	: mInfo(info)
	, mPort(port)
{
	mConfigurationTimeoutTimer.setSingleShot(true);
	mConfigurationTimeoutTimer.setInterval(configurationTimeout);
	connect(&mConfigurationTimeoutTimer, &QTimer::timeout, this, &Device::configurationTimerTimeoutSlot);
}

Device::~Device()
{
}

PortInfo const &Device::port() const
{
	return mPort;
}

DeviceInfo const &Device::deviceInfo() const
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

void Device::configurationCompleted(bool success)
{
	mConfigurationTimeoutTimer.stop();
	emit configured(success);
}

void Device::configurationTimerTimeoutSlot()
{
	emit configured(false);
}
