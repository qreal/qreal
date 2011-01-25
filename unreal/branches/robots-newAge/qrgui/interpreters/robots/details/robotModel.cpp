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
	connect(mRobotCommunicationInterface, SIGNAL(connected()), this, SLOT(connectedSlot()));
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

robotParts::TouchSensor *RobotModel::touchSensor(int port) const
{
	robotParts::TouchSensor *result = dynamic_cast<robotParts::TouchSensor *>(mSensors[port]);
	return result;
}

void RobotModel::clear()
{
}

void RobotModel::configureSensors(SensorType::SensorType const &port1
		, SensorType::SensorType const &port2
		, SensorType::SensorType const &port3
		, SensorType::SensorType const &port4)
{
	configureSensor(port1, 0);
	configureSensor(port2, 1);
	configureSensor(port3, 2);
	configureSensor(port4, 3);
}

void RobotModel::configureSensor(SensorType::SensorType const &sensorType, int port)
{
	delete mSensors[port];
	mSensors[port] = NULL;
	switch (sensorType) {
	case SensorType::unused:
		break;
	case SensorType::touchBoolean:
		mSensors[port] = new robotParts::TouchSensor(mRobotCommunicationInterface, port);
		break;
	case SensorType::touchRaw:
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

void RobotModel::connectedSlot()
{
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
		emit connected();
	}
}

void RobotModel::sensorConfigurationDoneSlot()
{
	--mSensorsToConfigure;
	if (mSensorsToConfigure == 0) {
		qDebug() << "Sensor configuration done";
		emit connected();
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
