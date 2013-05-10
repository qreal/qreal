#include "bluetoothGyroscopeSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothGyroscopeSensorImplementation::BluetoothGyroscopeSensorImplementation(RobotCommunicator *robotCommunicationInterface
																			   , inputPort::InputPortEnum port)
																		   : BluetoothSensorImplementation(robotCommunicationInterface
																		   , sensorType::sound, lowLevelSensorType::SOUND_DBA
																		   , sensorMode::RAWMODE, port)
																	   {
																	   }
void BluetoothGyroscopeSensorImplementation::read()
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
void BluetoothGyroscopeSensorImplementation::sensorSpecificProcessResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(tracer::sensors, "BluetoothGyroscopeSensorImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else {
		mState = idle;
		emit response(0xff & reading[14]);
	}
}
