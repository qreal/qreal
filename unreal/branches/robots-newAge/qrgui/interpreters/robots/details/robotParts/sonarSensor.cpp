#include "sonarSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

SonarSensor::SonarSensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelInputPort::InputPortEnum const &port)
	: I2CSensor(robotCommunicationInterface, lowLevelSensorType::LOWSPEED_9V, sensorMode::RAWMODE, port)
{
}

void SonarSensor::read()
{
	if (mState == pending)
		return;

	mState = pending;

	setMode(sonarMode::SINGLE_SHOT);
}

void SonarSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		qDebug() << "Something is wrong, response is empty";
	} else if (reading.size() == 1 && reading[0] == 0) {
		// Sensor configured, now we can send actual request.
		QByteArray command(2, 0);
		command[0] = sonarRegisters::SONAR_ADDRESS;
		command[1] = sonarRegisters::RESULT_1;
		sendCommand(command, 1);
	} else if (reading.size() == 1 && reading[0] != 0) {
		qDebug() << "Reading malformed:" << static_cast<unsigned int>(reading[0]);
	} else {
		qDebug() << "Data received" << (0xff & reading[1]) << "cm";
		mState = idle;
		emit response(0xff & reading[1]);
	}
}

void SonarSensor::setMode(sonarMode::SonarModeEnum const &mode)
{
	writeRegister(sonarRegisters::COMMAND, mode);
}

void SonarSensor::writeRegister(sonarRegisters::SonarRegistersEnum const &reg, int value)
{
	QByteArray command(3, 0);
	command[0] = sonarRegisters::SONAR_ADDRESS;
	command[1] = reg;
	command[2] = value;

	sendCommand(command, 0);
}
