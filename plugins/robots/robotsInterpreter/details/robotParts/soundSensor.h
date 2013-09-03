#pragma once

#include <QtCore/QObject>

#include "sensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class SoundSensor : public Sensor
{
	Q_OBJECT

public:
	SoundSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, robots::enums::inputPort::InputPortEnum const port);
};

}
}
}
}
}
