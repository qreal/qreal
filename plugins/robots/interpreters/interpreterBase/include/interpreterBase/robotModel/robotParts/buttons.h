#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Buttons : public Device
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Buttons"))
	Q_CLASSINFO("direction", "input")

public:
	Buttons(DeviceInfo const &info, PortInfo const &port);

	/// Ask buttons to provide current state. Returns immediately. When reading is done, signal "response" is emitted
	/// with state as a parameter.
	virtual void read() = 0;
};

}
}
}
