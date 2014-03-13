#include "sonarSensor.h"
#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

QString SonarSensor::friendlyName()
{
	return tr("Sonar sensor");
}

SonarSensor::SonarSensor(DeviceInfo const &info, PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::RangeSensor(info, port)
	, mImplementation(*this, robotCommunicator, port
			, enums::lowLevelSensorType::LOWSPEED_9V, enums::sensorMode::RAWMODE)
	, mI2C(robotCommunicator)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &SonarSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured
			, this, &SonarSensor::configurationCompleted);
}

void SonarSensor::read()
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
	setMode(enums::sonarMode::SINGLE_SHOT);
}

void SonarSensor::doConfiguration()
{
	mImplementation.configure();
}

void SonarSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse"
				, "Something is wrong, response is empty");
	} else if (reading.size() == 1 && reading[0] == 0) {
		// Sensor configured, now we can send actual request.
		QByteArray command(2, 0);
		command[0] = enums::sonarRegisters::SONAR_ADDRESS;
		command[1] = enums::sonarRegisters::RESULT_1;
		sendCommand(command, 1);
	} else if (reading.size() == 1 && reading[0] != 0) {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse"
				, "Reading malformed: " + QString::number(static_cast<unsigned int>(reading[0])));
	} else {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse"
				, "Data received: " + QString::number(0xff & reading[1]) + " cm");
		mImplementation.setState(NxtInputDevice::idle);
		emit newData(0xff & reading[1]);
	}
}

void SonarSensor::setMode(nxtKitInterpreter::enums::sonarMode::SonarModeEnum mode)
{
	writeRegister(enums::sonarRegisters::COMMAND, mode);
}

void SonarSensor::writeRegister(nxtKitInterpreter::enums::sonarRegisters::SonarRegistersEnum reg, int value)
{
	QByteArray command(3, 0);
	command[0] = enums::sonarRegisters::SONAR_ADDRESS;
	command[1] = reg;
	command[2] = value;

	sendCommand(command, 0);
}

void SonarSensor::sendCommand(QByteArray const &command, int responseSize)
{
	mI2C.sendI2C(this, command, responseSize, port());
}
