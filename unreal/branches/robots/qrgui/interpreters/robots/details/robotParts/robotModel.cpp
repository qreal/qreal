#include "robotModel.h"
#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations;

RobotModel::RobotModel(AbstractRobotModelImplementation *robotImpl)
	: mRobotImpl(robotImpl)
	, mBrick(&robotImpl->brick())
	, mMotorA(0, &robotImpl->motorA())
	, mMotorB(1, &robotImpl->motorB())
	, mMotorC(2, &robotImpl->motorC())
{
}

RobotModel::~RobotModel()
{
	delete mRobotImpl;
}

robotParts::Brick &RobotModel::brick()
{
	return mBrick;
}

robotParts::TouchSensor *RobotModel::touchSensor(inputPort::InputPortEnum const &port) const
{
	return new robotParts::TouchSensor(mRobotImpl->sensors().at(port), port);
}

robotParts::SonarSensor *RobotModel::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return new robotParts::SonarSensor(mRobotImpl->sensors().at(port), port);
}

robotParts::ColorSensor *RobotModel::colorSensor(inputPort::InputPortEnum const &port) const
{
	return new robotParts::ColorSensor(mRobotImpl->sensors().at(port), port);
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
	mRobotImpl->configureSensor(sensorType, port);
}

void RobotModel::init()
{
	mRobotImpl->init();
}

void RobotModel::stopRobot()
{
	mRobotImpl->stopRobot();
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

robotImplementations::AbstractRobotModelImplementation &RobotModel::robotImpl()
{
	return *mRobotImpl;
}

void RobotModel::setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl)
{
	mRobotImpl = robotImpl;
}
