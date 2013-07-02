#include "bluetoothLightSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

int const maxLightValue = 1023;

BluetoothLightSensorImplementation::BluetoothLightSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, inputPort::InputPortEnum const port)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType::light, lowLevelSensorType::LIGHT_ACTIVE, sensorMode::RAWMODE, port)
{
}

void BluetoothLightSensorImplementation::read()
{
	if (!mIsConfigured) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mState == pending) {
		return;
	}

	mState = pending;
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::GETINPUTVALUES;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 18);
}

void BluetoothLightSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mState = idle;
	int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	Tracer::debug(tracer::sensors, "BluetoothLightSensorImplementation::sensorSpecificProcessResponse", QString::number(sensorValue));
	emit response(sensorValue * 100 / maxLightValue);
}
