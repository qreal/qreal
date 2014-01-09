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
	BluetoothSonarSensorImplementation(
			RobotCommunicator *robotCommunicationInterface
			, robots::enums::inputPort::InputPortEnum const port
			);

	virtual void read();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);

private:
	virtual void setMode(enums::sonarMode::SonarModeEnum const &mode);
	virtual void writeRegister(enums::sonarRegisters::SonarRegistersEnum const &reg, int value);
	virtual void sendCommand(QByteArray const &command, int responseSize);//from i2c
};

}
}
}
}
}
}
