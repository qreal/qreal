#include "i2cSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

I2CSensor::I2CSensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, lowLevelInputPort::InputPortEnum const &port)
	: Sensor(robotCommunicationInterface, lowLevelSensorType, sensorMode, port)
{
}

void I2CSensor::sendCommand(QByteArray const &command, int responseSize)
{
	mRobotCommunicationInterface->sendI2C(this, command, responseSize, static_cast<inputPort::InputPortEnum>(mPort));
}
