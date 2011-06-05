#pragma once

#include "sensorsConfiguration.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class IConfigurableRobotModel
{
public:
	virtual SensorsConfiguration &configuration() = 0;
};

}
}
}
}
}
