#pragma once

#include <QtCore/QObject>

#include "sensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class TouchSensor : public Sensor
{
	Q_OBJECT

public:
	TouchSensor(RobotCommunicationInterface *robotCommunicationInterface
			, lowLevelInputPort::InputPortEnum const &port);
};

}
}
}
}
}
