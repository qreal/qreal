#pragma once
#include "nullSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullLightSensorImplementation : public NullSensorImplementation
{
	Q_OBJECT

public:
	NullLightSensorImplementation(inputPort::InputPortEnum const &port);

protected slots:
	virtual void timerTimeout();
};

}
}
}
}
}
}
