#include "sensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

Sensor::Sensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelSensorType::SensorTypeEnum const &sensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, lowLevelInputPort::InputPortEnum const &port)
	: RobotPart(robotCommunicationInterface)
	, mPort(port)
	, mState(idle)
	, mSensorType(sensorType)
	, mSensorMode(sensorMode)
	, mIsConfigured(false)
	, mResetDone(false)
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
	if (reading.isEmpty()) {
		mState = idle;
		if (mResetDone) {
			qDebug() << "Response is empty, seems to be a connection failure";
			emit failure();
		} else {
			if (mIsConfigured) {
				mResetDone = true;
				emit configured();
			}
			else {
				QByteArray command(5, 0);
				command[0] = 0x03;
				command[1] = 0x00;
				command[2] = telegramType::directCommandNoResponse;
				command[3] = commandCode::RESETINPUTSCALEDVALUE;
				command[4] = mPort;
				mRobotCommunicationInterface->send(this, command, 0);
			}
			mIsConfigured = true;
		}
	} else if (reading.size() >= 4 && reading[3] == commandCode::SETINPUTMODE) {
		mState = idle;
		qDebug() << "Response is a configuration response package";
		qDebug() << "Status byte is:" << static_cast<int>(reading[4]);
		mIsConfigured = true;
		emit configured();
	} else {
		sensorSpecificProcessResponse(reading);
	}
}

void Sensor::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandNoResponse;
	command[3] = commandCode::SETINPUTMODE;
	command[4] = mPort;
	command[5] = mSensorType;
	command[6] = mSensorMode;
	mRobotCommunicationInterface->send(this, command, 0);
}
