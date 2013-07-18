#pragma once

#include "nullSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullSoundSensorImplementation : public NullSensorImplementation
{
	Q_OBJECT

public:
	NullSoundSensorImplementation(robots::enums::inputPort::InputPortEnum const port);

protected slots:
	virtual void timerTimeout();
};

}
}
}
}
}
}
