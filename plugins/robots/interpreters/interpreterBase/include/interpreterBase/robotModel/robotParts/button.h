#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Button : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "button")
	Q_CLASSINFO("friendlyName", tr("Button"))

public:
	Button(DeviceInfo const &info, PortInfo const &port);

private:
	void doConfiguration() override;
};

}
}
}
