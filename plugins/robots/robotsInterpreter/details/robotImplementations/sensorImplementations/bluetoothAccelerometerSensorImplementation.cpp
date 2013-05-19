#include "bluetoothAccelerometerSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;


BluetoothAccelerometerSensorImplementation::BluetoothAccelerometerSensorImplementation(RobotCommunicator *robotCommunicationInterface
	, inputPort::InputPortEnum const port)
	 : BluetoothSensorImplementation(robotCommunicationInterface
	 , sensorType::accelerometer, lowLevelSensorType::ANGLE
	, sensorMode::RAWMODE, port)
{
}

void BluetoothAccelerometerSensorImplementation::read()
{
	if (!mIsConfigured) {
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
void BluetoothAccelerometerSensorImplementation::sensorSpecificProcessResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(tracer::sensors, "BluetoothAccelerometerSensorImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else {
		mState = idle;
		emit response(0xff & reading[14]);
	}
}
