#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Motor : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "motor")
	Q_CLASSINFO("friendlyName", tr("Motor"))
	Q_CLASSINFO("direction", "output")

public:
	Motor(DeviceInfo const &info, PortInfo const &port);

	virtual void on(int speed);
	virtual void stop();
	virtual void off();
};

}
}
}
