#include "bluetoothAccelerometerSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothAccelerometerSensorImplementation::BluetoothAccelerometerSensorImplementation(
		RobotCommunicator *robotCommunicationInterface
		, robots::enums::inputPort::InputPortEnum const port
		)
		: BluetoothSensorImplementation(robotCommunicationInterface
				, robots::enums::sensorType::accelerometer
				, enums::lowLevelSensorType::ANGLE
				, enums::sensorMode::RAWMODE
				, port
				)
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
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 18);
}

void BluetoothAccelerometerSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(
				tracer::enums::sensors
				, "BluetoothAccelerometerSensorImplementation::sensorSpecificProcessResponse"
				, "Something is wrong, response is empty"
				);
	} else {
		mState = idle;
		emit response((0xff & reading[14]) << 8 | (0xff & reading[15]));
	}
}
