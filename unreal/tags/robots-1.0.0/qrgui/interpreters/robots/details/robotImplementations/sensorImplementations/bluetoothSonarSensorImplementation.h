#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothSonarSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT
public:
	BluetoothSonarSensorImplementation(RobotCommunicationInterface *robotCommunicationInterface
			, inputPort::InputPortEnum const &port);
	virtual void read();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
private:
	virtual void setMode(sonarMode::SonarModeEnum const &mode);
	virtual void writeRegister(sonarRegisters::SonarRegistersEnum const &reg, int value);
	virtual void sendCommand(QByteArray const &command, int responseSize);//from i2c
};

}
}
}
}
}
}
