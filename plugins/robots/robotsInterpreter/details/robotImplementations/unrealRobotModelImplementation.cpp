#include "unrealRobotModelImplementation.h"
#include "../tracer.h"
#include "../d2RobotModel/d2ModelTimer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace details::robotImplementations;
using namespace details::d2Model;

UnrealRobotModelImplementation::UnrealRobotModelImplementation(D2RobotModel *d2RobotModel)
	: AbstractRobotModelImplementation()
	, mD2Model(d2RobotModel)
	, mBrick(d2RobotModel)
	, mMotorA(0, d2RobotModel)
	, mMotorB(1, d2RobotModel)
	, mMotorC(2, d2RobotModel)
	, mDisplay(d2RobotModel)
	, mEncoderA(enums::outputPort::port1, d2RobotModel)
	, mEncoderB(enums::outputPort::port2, d2RobotModel)
	, mEncoderC(enums::outputPort::port3, d2RobotModel)
{
	mActiveWaitingTimer.setInterval(500);
	mActiveWaitingTimer.setSingleShot(true);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
	mDisplay.attachToPaintWidget();
}

 UnrealRobotModelImplementation::~UnrealRobotModelImplementation()
{
	delete mD2Model;
}

brickImplementations::UnrealBrickImplementation &UnrealRobotModelImplementation::brick()
{
	return mBrick;
}

displayImplementations::UnrealDisplayImplementation &UnrealRobotModelImplementation::display()
{
	return mDisplay;
}

sensorImplementations::UnrealTouchSensorImplementation *UnrealRobotModelImplementation::touchSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealTouchSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealSonarSensorImplementation *UnrealRobotModelImplementation::sonarSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealSonarSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealColorSensorImplementation *UnrealRobotModelImplementation::colorSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealColorSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealLightSensorImplementation *UnrealRobotModelImplementation::lightSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealLightSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealSoundSensorImplementation *UnrealRobotModelImplementation::soundSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealSoundSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealGyroscopeSensorImplementation *UnrealRobotModelImplementation::gyroscopeSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealGyroscopeSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealAccelerometerSensorImplementation *UnrealRobotModelImplementation::accelerometerSensor(robots::enums::inputPort::InputPortEnum const port) const
{
	return dynamic_cast<sensorImplementations::UnrealAccelerometerSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

void UnrealRobotModelImplementation::addTouchSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addTouchSensor", "Configuring touch sensor on port " + QString::number(port));
	sensorImplementations::UnrealTouchSensorImplementation *sensor = new sensorImplementations::UnrealTouchSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addSonarSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addSonarSensor", "Configuring sonar sensor on port " + QString::number(port));
	sensorImplementations::UnrealSonarSensorImplementation *sensor = new sensorImplementations::UnrealSonarSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addColorSensor(
		robots::enums::inputPort::InputPortEnum const port
		, enums::lowLevelSensorType::SensorTypeEnum mode
		, robots::enums::sensorType::SensorTypeEnum const &sensorType
		)
{
	Q_UNUSED(mode)
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addColorSensor", "Configuring color sensor on port " + QString::number(port));
	sensorImplementations::UnrealColorSensorImplementation *sensor = new sensorImplementations::UnrealColorSensorImplementation(port, mD2Model, sensorType);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addLightSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addLightSensor", "Configuring light sensor on port " + QString::number(port));
	sensorImplementations::UnrealLightSensorImplementation *sensor = new sensorImplementations::UnrealLightSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addSoundSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addSoundSensor", "Configuring light sensor on port " + QString::number(port));
	sensorImplementations::UnrealSoundSensorImplementation *sensor = new sensorImplementations::UnrealSoundSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addGyroscopeSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addGyroscopeSensor", "Configuring gyroscope sensor on port " + QString::number(port));
	sensorImplementations::UnrealGyroscopeSensorImplementation *sensor = new sensorImplementations::UnrealGyroscopeSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addAccelerometerSensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::addAccelerometerSensor", "Configuring accelerometer sensor on port " + QString::number(port));
	sensorImplementations::UnrealAccelerometerSensorImplementation *sensor = new sensorImplementations::UnrealAccelerometerSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::init()
{
	AbstractRobotModelImplementation::init();
	mActiveWaitingTimer.start();
	mD2Model->startInit();
}

void UnrealRobotModelImplementation::timerTimeout()
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::timerTimeout", "Configuring sensors");
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
	mSensorsConfigurer.unlockConfiguring();
}

void UnrealRobotModelImplementation::sensorConfigurationDoneSlot()
{
	Tracer::debug(tracer::enums::initialization, "UnrealRobotModelImplementation::sensorConfigurationDoneSlot", "");
	connectRobot();
}

void UnrealRobotModelImplementation::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
	mD2Model->stopRobot();
}

motorImplementations::UnrealMotorImplementation &UnrealRobotModelImplementation::motorA()
{
	return mMotorA;
}

motorImplementations::UnrealMotorImplementation &UnrealRobotModelImplementation::motorB()
{
	return mMotorB;
}

motorImplementations::UnrealMotorImplementation &UnrealRobotModelImplementation::motorC()
{
	return mMotorC;
}

sensorImplementations::UnrealEncoderImplementation &UnrealRobotModelImplementation::encoderA()
{
	return mEncoderA;
}

sensorImplementations::UnrealEncoderImplementation &UnrealRobotModelImplementation::encoderB()
{
	return mEncoderB;
}

sensorImplementations::UnrealEncoderImplementation &UnrealRobotModelImplementation::encoderC()
{
	return mEncoderC;
}

void UnrealRobotModelImplementation::startInterpretation()
{
	mDisplay.clearScreen();
	mD2Model->showModelWidget();
	mD2Model->startInterpretation();
}

TimelineInterface *UnrealRobotModelImplementation::timeline()
{
	return mD2Model->timeline();
}
