#include "sonarSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

SonarSensor::SonarSensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelInputPort::InputPortEnum const &port)
	: I2CSensor(robotCommunicationInterface, lowLevelSensorType::SONAR_METRIC, sensorMode::RAWMODE, port)
{
}

void SonarSensor::read()
{
	if (mState == pending)
		return;

	qDebug() << "Setting sensor mode";
	setMode(sonarMode::SINGLE_SHOT);
}

void SonarSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	qDebug() << "Response received";
	if (reading.size() == 1 && reading[0] == 0) {
		// Sensor configured, now we can send actual request.
		qDebug() << "Mode set";
		QByteArray command(2, 0);
		command[0] = sonarRegisters::SONAR_ADDRESS;
		command[1] = sonarRegisters::RESULT_1;
		sendCommand(command, 1);
	} else {
		qDebug() << "Data received";
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
