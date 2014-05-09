#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Display : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "display")
	Q_CLASSINFO("friendlyName", tr("Display"))
	Q_CLASSINFO("direction", "output")

public:
	Display(DeviceInfo const &info, PortInfo const &port);

	virtual void clearScreen() = 0;
};

}
}
}
