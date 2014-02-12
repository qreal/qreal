#include "nxtInputDevice.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace utils;

NxtInputDevice::NxtInputDevice(QObject *instance
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, interpreterBase::robotModel::PortInfo const &port
		, enums::lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, enums::sensorMode::SensorModeEnum const &sensorMode)
	: mInstance(instance)
	, mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
	, mState(idle)
	, mIsConfigured(false)
	, mResetDone(false)
{
}

void NxtInputDevice::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee == mInstance) {
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
		onDeviceConfigured();
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
		mRobotCommunicator.send(mInstance, command, 5);

		mIsConfigured = true;
	} else {
		sensorSpecificProcessResponse(reading);
	}
}

void NxtInputDevice::onDeviceConfigured()
{
}

void NxtInputDevice::configureNxtDevice()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::SETINPUTMODE;
	command[4] = mLowLevelPort;
	command[5] = mLowLevelSensorType;
	command[6] = mSensorMode;
	mRobotCommunicator.send(mInstance, command, 5);
}
