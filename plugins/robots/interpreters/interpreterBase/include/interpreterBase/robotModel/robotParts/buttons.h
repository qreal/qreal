#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Buttons : public Device
{
	Q_OBJECT

public:
	static QString friendlyName();
	static DeviceInfo::Direction direction();

	Buttons(DeviceInfo const &info, PortInfo const &port);

	/// Ask buttons to provide current state. Returns immediately. When reading is done, signal "responce" is emitted
	/// with state as a parameter.
	virtual void read() = 0;
};

}
}
}
