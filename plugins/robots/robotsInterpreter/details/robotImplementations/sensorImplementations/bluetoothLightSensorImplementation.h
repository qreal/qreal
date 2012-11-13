#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothLightSensorImplementation : public BluetoothSensorImplementation
{
public:
	BluetoothLightSensorImplementation(RobotCommunicator *robotCommunicationInterface
			, inputPort::InputPortEnum const &port);
	virtual void read();
};
}
}
}
}
}
}
