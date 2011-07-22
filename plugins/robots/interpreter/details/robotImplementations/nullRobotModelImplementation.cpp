#include "nullRobotModelImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

NullRobotModelImplementation::NullRobotModelImplementation()
	: AbstractRobotModelImplementation()
	, mBrick()
	, mMotorA(0)
	, mMotorB(1)
	, mMotorC(2)
	, mEncoderA(outputPort::port1)
	, mEncoderB(outputPort::port2)
	, mEncoderC(outputPort::port3)
{
	mActiveWaitingTimer.setInterval(100);
	mActiveWaitingTimer.setSingleShot(true);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

brickImplementations::NullBrickImplementation &NullRobotModelImplementation::brick()
{
	return mBrick;
}

sensorImplementations::NullTouchSensorImplementation *NullRobotModelImplementation::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullTouchSensorImplementation *>(mSensors[port]);
}

sensorImplementations::NullSonarSensorImplementation *NullRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullSonarSensorImplementation *>(mSensors[port]);
}

sensorImplementations::NullColorSensorImplementation *NullRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullColorSensorImplementation *>(mSensors[port]);
}

void NullRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = new sensorImplementations::NullTouchSensorImplementation(port);
}

void NullRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = new sensorImplementations::NullSonarSensorImplementation(port);
}

void NullRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode)
{
	Q_UNUSED(mode);
	mSensors[port] = new sensorImplementations::NullColorSensorImplementation(port);
}

void NullRobotModelImplementation::init()
{
	AbstractRobotModelImplementation::init();
	mActiveWaitingTimer.start();
}

void NullRobotModelImplementation::timerTimeout()
{
	emit connected(true);
}

void NullRobotModelImplementation::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
}

motorImplementations::NullMotorImplementation &NullRobotModelImplementation::motorA()
{
	return mMotorA;
}

motorImplementations::NullMotorImplementation &NullRobotModelImplementation::motorB()
{
	return mMotorB;
}

motorImplementations::NullMotorImplementation &NullRobotModelImplementation::motorC()
{
	return mMotorC;
}

sensorImplementations::NullEncoderImplementation &NullRobotModelImplementation::encoderA()
{
	return mEncoderA;
}

sensorImplementations::NullEncoderImplementation &NullRobotModelImplementation::encoderB()
{
	return mEncoderB;
}

sensorImplementations::NullEncoderImplementation &NullRobotModelImplementation::encoderC()
{
	return mEncoderC;
}

