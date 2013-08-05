#pragma once

#include "nullSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullAccelerometerSensorImplementation : public NullSensorImplementation
{
	Q_OBJECT

public:
	NullAccelerometerSensorImplementation(robots::enums::inputPort::InputPortEnum const port);

protected slots:
	virtual void timerTimeout();
};

}
}
}
}
}
}
