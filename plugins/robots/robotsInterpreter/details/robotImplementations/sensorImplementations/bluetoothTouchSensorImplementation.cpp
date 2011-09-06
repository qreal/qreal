#include "bluetoothTouchSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

BluetoothTouchSensorImplementation::BluetoothTouchSensorImplementation(RobotCommunication *robotCommunicationInterface
		, inputPort::InputPortEnum const &port)
	: BluetoothSensorImplementation(robotCommunicationInterface, lowLevelSensorType::SWITCH, sensorMode::BOOLEANMODE, port)
{
}

void BluetoothTouchSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mState = idle;
	if (reading[4] == 0 && reading[14] == 1)  // Sensor is pressed.
		emit response(1);
	else
		emit response(0);
}

void BluetoothTouchSensorImplementation::read()
{
	if (mState == pending)
		return;

	mState = pending;
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::GETINPUTVALUES;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 18);
}
