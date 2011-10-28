#pragma once

#include "sensorsConfiguration.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class RobotModelInterface
{
public:
	virtual SensorsConfiguration &configuration() = 0;
	virtual void clear() = 0;
	virtual void rotateOn(double angle) = 0;
	virtual double rotateAngle() const = 0;
};

}
}
}
}
}
