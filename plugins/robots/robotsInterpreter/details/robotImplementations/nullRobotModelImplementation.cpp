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
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
}

brickImplementations::NullBrickImplementation &NullRobotModelImplementation::brick()
{
	return mBrick;
}

sensorImplementations::NullTouchSensorImplementation *NullRobotModelImplementation::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullTouchSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullSonarSensorImplementation *NullRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullSonarSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullColorSensorImplementation *NullRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::NullColorSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

void NullRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
	sensorImplementations::NullTouchSensorImplementation *sensor = new sensorImplementations::NullTouchSensorImplementation(port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	sensorImplementations::NullSonarSensorImplementation *sensor = new sensorImplementations::NullSonarSensorImplementation(port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType)
{
	Q_UNUSED(mode);
	sensorImplementations::NullColorSensorImplementation *sensor = new sensorImplementations::NullColorSensorImplementation(port, sensorType);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::init()
{
	AbstractRobotModelImplementation::init();
	mActiveWaitingTimer.start();
}

void NullRobotModelImplementation::timerTimeout()
{
	mSensorsConfigurer.unlockConfiguring();
}

void NullRobotModelImplementation::sensorConfigurationDoneSlot()
{
	connectRobot();
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
