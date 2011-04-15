#pragma once
#include <QtCore/QObject>
#include "sensor.h"
#include "../../sensorConstants.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class SonarSensor : public Sensor
{
	Q_OBJECT
public:
	SonarSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port);
};

}
}
}
}
}
