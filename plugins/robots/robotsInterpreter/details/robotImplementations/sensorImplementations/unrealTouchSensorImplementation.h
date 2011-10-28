#pragma once
#include "unrealSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class UnrealTouchSensorImplementation : public UnrealSensorImplementation
{
	Q_OBJECT
public:
	UnrealTouchSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model);
	virtual void read();
};

}
}
}
}
}
}
