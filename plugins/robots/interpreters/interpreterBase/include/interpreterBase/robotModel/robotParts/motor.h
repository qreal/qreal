#pragma once

#include "pluggableDevice.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// @todo Generalize it.
class ROBOTS_INTERPRETER_BASE_EXPORT Motor : public PluggableDevice
{
	Q_OBJECT

public:
	virtual void on(int speed, bool breakMode = true);
	virtual void on(int speed, long unsigned int degrees, bool breakMode = true);
	virtual void stop(bool breakMode = true);
	virtual void off();
};

}
}
}
