#pragma once

#include "interpreterBase/robotModel/configurationInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT AbstractConfiguration : public ConfigurationInterface
{
	Q_OBJECT

public:
	void configureDevice(robotParts::PluggableDevice const &device, PortInfo const &port) override;

	robotParts::PluggableDevice &pluggableDevice(
			PortInfo const &port
			, PortDirection direction = defaultDirection) override;
};

}
}
