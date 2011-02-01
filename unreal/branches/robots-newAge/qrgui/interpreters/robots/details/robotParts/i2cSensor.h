#pragma once

#include "sensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class I2CSensor : public Sensor
{
	Q_OBJECT

public:
	I2CSensor(RobotCommunicationInterface *robotCommunicationInterface
			, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
			, sensorMode::SensorModeEnum const &sensorMode
			, lowLevelInputPort::InputPortEnum const &port);
	virtual void read() = 0;

protected:
	void sendCommand(QByteArray const &command, int responseSize);
};

}
}
}
}
}
