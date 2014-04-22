#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Speaker : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "speaker")
	Q_CLASSINFO("friendlyName", tr("Speaker"))
	Q_CLASSINFO("direction", "output")

public:
	Speaker(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
