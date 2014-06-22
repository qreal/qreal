#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Output device representing abstract robot display.
class ROBOTS_INTERPRETER_BASE_EXPORT Display : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "display")
	Q_CLASSINFO("friendlyName", tr("Display"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and predefined port on which display is configured.
	Display(DeviceInfo const &info, PortInfo const &port);

	/// Clears everything on a display.
	virtual void clearScreen() = 0;
};

}
}
}
