#include "bluetoothLightSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothLightSensorImplementation::BluetoothLightSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, inputPort::InputPortEnum const &port)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType::light, lowLevelSensorType::LIGHT_ACTIVE, sensorMode::RAWMODE, port)
{
}

void BluetoothLightSensorImplementation::read()
{

}
