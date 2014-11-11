#include "nxtInputDevice.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace utils;

NxtInputDevice::NxtInputDevice(
		utils::robotCommunication::RobotCommunicator &robotCommunicator
		, interpreterBase::robotModel::PortInfo const &port
		, enums::lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, enums::sensorMode::SensorModeEnum const &sensorMode)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
	, mState(idle)
	, mIsConfigured(false)
	, mResetDone(false)
{
	QObject::connect(&mRobotCommunicator, &utils::robotCommunication::RobotCommunicator::response
			, this, &NxtInputDevice::readingDone);
}

void NxtInputDevice::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee == this) {
		processResponse(reading);
	}
}

void NxtInputDevice::processResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		mState = idle;
		if (mResetDone) {
			Tracer::debug(Tracer::sensors, "BluetoothSensorImplementation::processResponse"
					, "Response is empty, seems to be a connection failure");
			// Just ignore connection failures for now
			// emit failure();
		}
	} else if (reading.size() >= 5 && reading[3] == enums::commandCode::RESETINPUTSCALEDVALUE) {
		Tracer::debug(Tracer::sensors, "BluetoothSensorImplementation::processResponse"
				, "Response is a reset input scaled value response package");
		Tracer::debug(Tracer::sensors, "BluetoothSensorImplementation::processResponse"
				, "Status byte is: " + QString::number(static_cast<int>(reading[4])));
		mState = idle;
		mResetDone = true;
		emit configured(true);
	} else if (reading.size() >= 5 && reading[3] == enums::commandCode::SETINPUTMODE) {
		mState = idle;
		Tracer::debug(Tracer::sensors, "BluetoothSensorImplementation::processResponse"
				, "Response is a configuration response package");
		Tracer::debug(Tracer::sensors, "BluetoothSensorImplementation::processResponse"
				, "Status byte is: " + QString::number(static_cast<int>(reading[4])));
		QByteArray command(5, 0);
		command[0] = 0x03;
		command[1] = 0x00;
		command[2] = enums::telegramType::directCommandResponseRequired;
		command[3] = enums::commandCode::RESETINPUTSCALEDVALUE;
		command[4] = mLowLevelPort;
		send(command, 5);

		mIsConfigured = true;
	} else {
		emit sensorSpecificProcessResponse(reading);
	}
}

void NxtInputDevice::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::SETINPUTMODE;
	command[4] = mLowLevelPort;
	command[5] = mLowLevelSensorType;
	command[6] = mSensorMode;
	send(command, 5);
}

void NxtInputDevice::send(QByteArray const &buffer, unsigned const responseSize)
{
	mRobotCommunicator.send(this, buffer, responseSize);
}

NxtInputDevice::State NxtInputDevice::state() const
{
	return mState;
}

void NxtInputDevice::setState(NxtInputDevice::State state)
{
	mState = state;
}

bool NxtInputDevice::isConfigured() const
{
	return mIsConfigured;
}

char NxtInputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}
