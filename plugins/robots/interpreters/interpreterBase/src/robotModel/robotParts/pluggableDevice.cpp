#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"

using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

PluggableDevice::PluggableDevice(PortInfo const &port)
	: mPort(port)
{
}

PluggableDevice::~PluggableDevice()
{
}

PortInfo const &PluggableDevice::port() const
{
	return mPort;
}
