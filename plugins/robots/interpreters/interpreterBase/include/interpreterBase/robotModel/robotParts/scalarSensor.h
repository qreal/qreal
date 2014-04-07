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
	ScalarSensor(DeviceInfo const &info, PortInfo const &port);
	virtual ~ScalarSensor();

signals:
	void newData(int reading);
};

}
}
}
