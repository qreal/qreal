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
	static QString friendlyName();

	ScalarSensor(DeviceInfo const &info, PortInfo const &port);
	virtual ~ScalarSensor();

signals:
	void newData(int reading);
};

}
}
}
