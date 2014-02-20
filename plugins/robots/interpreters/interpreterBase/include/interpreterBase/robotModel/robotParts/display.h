#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Display : public Device
{
	static DeviceInfo::Direction direction();

	Display(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
