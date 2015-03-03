#include "touchSensor.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

TouchSensor::TouchSensor(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::TouchSensor(info, port)
	, mImplementation(robotCommunicator, port, enums::lowLevelSensorType::SWITCH, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &TouchSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &TouchSensor::configurationCompleted);
}

void TouchSensor::sensorSpecificProcessResponse(const QByteArray &reading)
{
	mImplementation.setState(NxtInputDevice::idle);
	int sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	Tracer::debug(Tracer::sensors, "TouchSensor::sensorSpecificProcessResponse", QString::number(sensorValue));
	if (reading[4] == 0 && sensorValue < 500) {
		// Sensor is pressed.
		emit newData(1);
	} else {
		emit newData(0);
	}
}

void TouchSensor::read()
{
	if (!mImplementation.isConfigured()) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mImplementation.state() == NxtInputDevice::pending) {
		return;
	}

	mImplementation.setState(NxtInputDevice::pending);
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mImplementation.lowLevelPort();
	mImplementation.send(command, 18);
}

void TouchSensor::doConfiguration()
{
	mImplementation.configure();
}
