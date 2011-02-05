#pragma once

#include <QtCore/QObject>

#include "i2cSensor.h"
#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class SonarSensor : public I2CSensor
{
	Q_OBJECT

public:
	SonarSensor(RobotCommunicationInterface *robotCommunicationInterface
			, lowLevelInputPort::InputPortEnum const &port);
	virtual void read();

private:
	void setMode(sonarMode::SonarModeEnum const &mode);
	void writeRegister(sonarRegisters::SonarRegistersEnum const &reg, int value);

	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
}
