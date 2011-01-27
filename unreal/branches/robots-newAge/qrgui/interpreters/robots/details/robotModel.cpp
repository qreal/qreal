#include "robotModel.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details;

RobotModel::RobotModel(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
	, mBrick(robotCommunicationInterface)
	, mMotorA(0, robotCommunicationInterface)
	, mMotorB(1, robotCommunicationInterface)
	, mMotorC(2, robotCommunicationInterface)
	, mSensorsToConfigure(0)
{
	connect(mRobotCommunicationInterface, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	mSensors.resize(4);
}

RobotModel::~RobotModel()
{
	delete mRobotCommunicationInterface;
}

robotParts::Brick &RobotModel::brick()
{
	return mBrick;
}

robotParts::TouchSensor *RobotModel::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<robotParts::TouchSensor *>(mSensors[port]);
}

void RobotModel::clear()
{
}

void RobotModel::configureSensors(sensorType::SensorTypeEnum const &port1
		, sensorType::SensorTypeEnum const &port2
		, sensorType::SensorTypeEnum const &port3
		, sensorType::SensorTypeEnum const &port4)
{
	configureSensor(port1, inputPort::port1);
	configureSensor(port2, inputPort::port2);
	configureSensor(port3, inputPort::port3);
	configureSensor(port4, inputPort::port4);
}

void RobotModel::configureSensor(sensorType::SensorTypeEnum const &sensorType
		, inputPort::InputPortEnum const &port)
{
	delete mSensors[port];
	mSensors[port] = NULL;
	lowLevelInputPort::InputPortEnum lowLevelPort = static_cast<lowLevelInputPort::InputPortEnum>(port);
	switch (sensorType) {
	case sensorType::unused:
		break;
	case sensorType::touchBoolean:
		mSensors[port] = new robotParts::TouchSensor(mRobotCommunicationInterface, lowLevelPort);
		break;
	case sensorType::touchRaw:
		break;
	default:
		// TODO: Throw an exception
		break;
	}
}

void RobotModel::init()
{
	qDebug() << "Initializing robot model...";
	qDebug() << "Connecting to robot...";
	mRobotCommunicationInterface->connect();
}

void RobotModel::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
}

void RobotModel::connectedSlot(bool success)
{
	if (!success) {
		qDebug() << "Connection failed.";
		emit connected(false);
		return;
	}
	qDebug() << "Connected. Initializing sensors...";
	foreach (robotParts::Sensor *sensor, mSensors) {
		if (sensor != NULL) {
			connect(sensor, SIGNAL(configured()), this, SLOT(sensorConfigurationDoneSlot()));
			sensor->configure();
			++mSensorsToConfigure;
		}
	}
	if (mSensorsToConfigure == 0) {
		qDebug() << "No sensor configuration needed";
		emit connected(true);
	}
}

void RobotModel::sensorConfigurationDoneSlot()
{
	--mSensorsToConfigure;
	if (mSensorsToConfigure == 0) {
		qDebug() << "Sensor configuration done";
		emit connected(true);
	}
}

robotParts::Motor &RobotModel::motorA()
{
	return mMotorA;
}

robotParts::Motor &RobotModel::motorB()
{
	return mMotorB;
}

robotParts::Motor &RobotModel::motorC()
{
	return mMotorC;
}
