#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Speaker : public Device
{
	static QString friendlyName();
	static DeviceInfo::Direction direction();

	Speaker(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
