#pragma once

#include <QtCore/QObject>
#include "sensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class LightSensor : public Sensor
{
	Q_OBJECT
public:
	LightSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port);
};

}
}
}
}
}
