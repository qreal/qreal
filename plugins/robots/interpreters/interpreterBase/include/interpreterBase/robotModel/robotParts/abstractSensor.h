#pragma once

#include <interpreterBase/robotModel/robotParts/pluggableDevice.h>
#include <interpreterBase/interpreterBaseDeclSpec.h>

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT AbstractSensor : public PluggableDevice
{
public:
	virtual ~AbstractSensor() {}

	virtual void read() = 0;
};

}
}
}
