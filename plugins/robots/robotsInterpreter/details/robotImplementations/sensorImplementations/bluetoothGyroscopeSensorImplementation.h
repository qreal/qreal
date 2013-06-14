#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothGyroscopeSensorImplementation : public BluetoothSensorImplementation
{
public:
	BluetoothGyroscopeSensorImplementation(RobotCommunicator *robotCommunicationInterface
										   , inputPort::InputPortEnum port);
	virtual void read();
protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};
}
}
}
}
}
}
