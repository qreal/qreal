#include "nullRobotModelImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

NullRobotModelImplementation::NullRobotModelImplementation()
	: AbstractRobotModelImplementation()
	, mBrick()
	, mDisplay()
	, mMotorA(0)
	, mMotorB(1)
	, mMotorC(2)
	, mEncoderA(enums::outputPort::port1)
	, mEncoderB(enums::outputPort::port2)
	, mEncoderC(enums::outputPort::port3)
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

displayImplementations::NullDisplayImplementation &NullRobotModelImplementation::display()
{
	return mDisplay;
}

sensorImplementations::NullTouchSensorImplementation *NullRobotModelImplementation::touchSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullTouchSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullSonarSensorImplementation *NullRobotModelImplementation::sonarSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullSonarSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullColorSensorImplementation *NullRobotModelImplementation::colorSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullColorSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullLightSensorImplementation *NullRobotModelImplementation::lightSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullLightSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullSoundSensorImplementation *NullRobotModelImplementation::soundSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullSoundSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullGyroscopeSensorImplementation *NullRobotModelImplementation::gyroscopeSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullGyroscopeSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::NullAccelerometerSensorImplementation *NullRobotModelImplementation::accelerometerSensor(
		robots::enums::inputPort::InputPortEnum const port
		) const
{
	return dynamic_cast<sensorImplementations::NullAccelerometerSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

void NullRobotModelImplementation::addTouchSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullTouchSensorImplementation *sensor = new sensorImplementations::NullTouchSensorImplementation(port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addLightSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullLightSensorImplementation *sensor = new sensorImplementations::NullLightSensorImplementation(port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addColorSensor(
		robots::enums::inputPort::InputPortEnum const port
		, enums::lowLevelSensorType::SensorTypeEnum mode
		, robots::enums::sensorType::SensorTypeEnum const &sensorType)
{
	Q_UNUSED(mode);
	sensorImplementations::NullColorSensorImplementation *sensor
			= new sensorImplementations::NullColorSensorImplementation(port, sensorType);

	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addSonarSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullSonarSensorImplementation *sensor
			= new sensorImplementations::NullSonarSensorImplementation(port);

	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addSoundSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullSoundSensorImplementation *sensor
			= new sensorImplementations::NullSoundSensorImplementation(port);

	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addGyroscopeSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullGyroscopeSensorImplementation *sensor \
			= new sensorImplementations::NullGyroscopeSensorImplementation(port);

	mSensorsConfigurer.configureSensor(sensor, port);
}

void NullRobotModelImplementation::addAccelerometerSensor(robots::enums::inputPort::InputPortEnum const port)
{
	sensorImplementations::NullAccelerometerSensorImplementation *sensor
			= new sensorImplementations::NullAccelerometerSensorImplementation(port);

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

details::TimelineInterface *NullRobotModelImplementation::timeline()
{
	return &mTimeline;
}
