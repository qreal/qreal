#include "interpreterBase/robotModel/abstractConfiguration.h"

using namespace interpreterBase::robotModel;

void AbstractConfiguration::configureDevice(robotParts::PluggableDevice const &device, PortInfo const &port)
{
	// TODO: Implement.
}

robotParts::PluggableDevice &AbstractConfiguration::pluggableDevice(
		PortInfo const &port
		, PortDirection direction)
{
	// TODO: Implement.
	robotParts::PluggableDevice device;
	return device;
}
