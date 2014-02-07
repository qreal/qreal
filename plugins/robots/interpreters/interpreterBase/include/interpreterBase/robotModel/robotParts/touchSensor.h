#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT TouchSensor : public ScalarSensor
{
	Q_OBJECT

public:
	explicit TouchSensor(PluggableDeviceInfo const &info, PortInfo const &port)
		: ScalarSensor(info, port) {}

	void read();

protected:
	void doConfiguration();

private:
	/// @todo Temporary.
	int mCount = 0;
};

}
}
}
