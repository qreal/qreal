#include "sensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

Sensor::Sensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelSensorType::SensorTypeEnum const &sensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, lowLevelInputPort::InputPortEnum const &port)
	: RobotPart(robotCommunicationInterface)
	, mState(idle)
	, mPort(port)
	, mSensorType(sensorType)
	, mSensorMode(sensorMode)
	, mIsConfigured(false)
{
}

void Sensor::read()
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

void Sensor::processResponse(QByteArray const &reading)
{
	mState = idle;
	if (reading.isEmpty()) {
		qDebug() << "Response is empty, seems to be a connection failure";
		if (mIsConfigured)
			emit fail();
		else
			emit configured();
	} else if (reading[3] == commandCode::SETINPUTMODE) {
		qDebug() << "Response is a configuration response package";
		qDebug() << "Status byte is:" << static_cast<int>(reading[4]);
		mIsConfigured = true;
		emit configured();
	} else {
		qDebug() << "=========================================================";
		qDebug() << "Sensor reading:";
		qDebug() << "Byte 0 (package type):" << static_cast<int>(reading[2]);
		qDebug() << "Byte 1 (command code):" << static_cast<int>(reading[3]);
		qDebug() << "Byte 2 (status byte):" << static_cast<int>(reading[4]);
		qDebug() << "Byte 3 (input port):" << static_cast<int>(reading[5]);
		qDebug() << "Byte 4 (valid):" << static_cast<int>(reading[6]);
		qDebug() << "Byte 5 (calibrated):" << static_cast<int>(reading[7]);
		qDebug() << "Byte 6 (sensor type):" << static_cast<int>(reading[8]);
		qDebug() << "Byte 7 (sensor mode):" << static_cast<int>(reading[9]);
		qDebug() << "Byte 8 (raw A/D, lower byte):" << static_cast<int>(reading[10]);
		qDebug() << "Byte 9 (raw A/D, upper byte):" << static_cast<int>(reading[11]);
		qDebug() << "Byte 10 (normalized A/D, lower byte):" << static_cast<int>(reading[12]);
		qDebug() << "Byte 11 (normalized A/D, upper byte byte):" << static_cast<int>(reading[13]);
		qDebug() << "Byte 12 (scaled value, lower byte):" << static_cast<int>(reading[14]);
		qDebug() << "Byte 13 (scaled value, upper byte):" << static_cast<int>(reading[15]);
		qDebug() << "Byte 14 (calibrated value, lower byte):" << static_cast<int>(reading[16]);
		qDebug() << "Byte 15 (calibrated value, upper byte):" << static_cast<int>(reading[17]);
		if (reading[4] == 0 && reading[14] == 1)  // Sensor is pressed.
			emit response(1);
		else
			emit response(0);
		// TODO: Where is the raw data?
	}
}

void Sensor::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;  // reply needed
	command[3] = commandCode::SETINPUTMODE;
	command[4] = mPort;  // sensor port. always 0 for now.
	command[5] = mSensorType;
	command[6] = mSensorMode;
	mRobotCommunicationInterface->send(this, command, 5);
}
