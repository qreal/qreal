#include "touchSensor.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

TouchSensor::TouchSensor(interpreterBase::robotModel::PluggableDeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::TouchSensor(info, port)
	, NxtInputDevice(this, robotCommunicator, port, enums::lowLevelSensorType::SWITCH, enums::sensorMode::RAWMODE)

{
}

void TouchSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mState = idle;
	int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	Tracer::debug(Tracer::sensors, "TouchSensor::sensorSpecificProcessResponse", QString::number(sensorValue));
	if (reading[4] == 0 && sensorValue < 500) {
		// Sensor is pressed.
		emit newData(1);
	} else {
		emit newData(0);
	}
}

void TouchSensor::onDeviceConfigured()
{
	configurationCompleted(true);
}

void TouchSensor::read()
{
	if (!mIsConfigured) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
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
	command[4] = mLowLevelPort;
	mRobotCommunicator.send(this, command, 18);
}
