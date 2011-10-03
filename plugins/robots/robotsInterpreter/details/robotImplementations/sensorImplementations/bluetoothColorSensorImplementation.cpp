#include "bluetoothColorSensorImplementation.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

BluetoothColorSensorImplementation::BluetoothColorSensorImplementation(
		RobotCommunication *robotCommunicationInterface
		, inputPort::InputPortEnum const &port
		, lowLevelSensorType::SensorTypeEnum mode
		, sensorType::SensorTypeEnum const sensorType)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType, mode, sensorMode::RAWMODE, port)
{
}

void BluetoothColorSensorImplementation::read()
{
	if (!mIsConfigured) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

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

void BluetoothColorSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		qDebug() << "Something is wrong, response is empty";
	} else {
		qDebug() << "Data received"
				<< (0xff & reading[8]) << (0xff & reading[9])
				<< (0xff & reading[10]) << (0xff & reading[11])
				<< (0xff & reading[12]) << (0xff & reading[13])
				<< (0xff & reading[14]) << (0xff & reading[15])
		;
		mState = idle;
		if (mLowLevelSensorType == lowLevelSensorType::COLORFULL) {
			emit response(0xff & reading[14]);  // Scaled value, used in ColorFull mode.
		} else {
			emit response((0xff & reading[10]) | ((0xff & reading[11]) << 8));
		}
	}
}

void BluetoothColorSensorImplementation::reconfigure(lowLevelSensorType::SensorTypeEnum mode)
{
	mLowLevelSensorType = mode;
	mIsConfigured = false;
	mResetDone = false;
	configure();
}
