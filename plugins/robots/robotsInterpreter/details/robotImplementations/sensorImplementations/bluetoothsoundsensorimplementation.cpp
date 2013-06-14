#include "bluetoothSoundSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

int const soundMaxValue = 1023;

BluetoothSoundSensorImplementation::BluetoothSoundSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, inputPort::InputPortEnum const port)
	: BluetoothSensorImplementation(robotCommunicationInterface
	, sensorType::sound, lowLevelSensorType::SOUND_DBA
	, sensorMode::RAWMODE, port)
{
}

void BluetoothSoundSensorImplementation::read()
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

void BluetoothSoundSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(tracer::sensors, "BluetoothSoundSensorImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else {
		int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
		mState = idle;
		emit response(sensorValue * 100 / soundMaxValue);
	}
}
