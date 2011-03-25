#pragma once
#include "nullSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullColorSensorImplementation : public NullSensorImplementation
{
	Q_OBJECT

public:
	NullColorSensorImplementation(inputPort::InputPortEnum const &port);

protected slots:
	virtual void timerTimeout();
};

}
}
}
}
}
}
