#pragma once

#include "abstractSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT ScalarSensor : public AbstractSensor
{
	Q_OBJECT

public:
	explicit ScalarSensor(PluggableDeviceInfo const &info, PortInfo const &port)
		: AbstractSensor(info, port) {}

signals:
	void newData(int reading);
};

}
}
}
