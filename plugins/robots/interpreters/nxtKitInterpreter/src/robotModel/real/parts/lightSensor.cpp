#include "lightSensor.h"
#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

int const maxLightValue = 1023;

LightSensor::LightSensor(DeviceInfo const &info, PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::LightSensor(info, port)
	, mImplementation(robotCommunicator, port
			, enums::lowLevelSensorType::LIGHT_ACTIVE, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &LightSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &LightSensor::configurationCompleted);
}

void LightSensor::read()
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

void LightSensor::doConfiguration()
{
	mImplementation.configure();
}

void LightSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mImplementation.setState(NxtInputDevice::idle);
	int const sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
	utils::Tracer::debug(utils::Tracer::sensors
			, "BluetoothLightSensorImplementation::sensorSpecificProcessResponse", QString::number(sensorValue));
	emit newData(sensorValue * 100 / maxLightValue);
}
