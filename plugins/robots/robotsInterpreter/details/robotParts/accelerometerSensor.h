#pragma once

#include <QtCore/QObject>
#include "sensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class AccelerometerSensor : public Sensor
{
	Q_OBJECT
public:
	AccelerometerSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const port);
};

}
}
}
}
}
