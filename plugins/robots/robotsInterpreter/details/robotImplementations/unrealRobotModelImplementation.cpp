#include "unrealRobotModelImplementation.h"

#include <QtCore/QDebug>

#include "../tracer.h"

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
	, mEncoderA(outputPort::port1, d2RobotModel)
	, mEncoderB(outputPort::port2, d2RobotModel)
	, mEncoderC(outputPort::port3, d2RobotModel)
{
	mActiveWaitingTimer.setInterval(500);
	mActiveWaitingTimer.setSingleShot(true);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
}

 UnrealRobotModelImplementation::~UnrealRobotModelImplementation()
{
	delete mD2Model;
}

brickImplementations::UnrealBrickImplementation &UnrealRobotModelImplementation::brick()
{
	return mBrick;
}

sensorImplementations::UnrealTouchSensorImplementation *UnrealRobotModelImplementation::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::UnrealTouchSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealSonarSensorImplementation *UnrealRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::UnrealSonarSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::UnrealColorSensorImplementation *UnrealRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::UnrealColorSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

void UnrealRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
	Tracer::debug(tracer::initialization, "UnrealRobotModelImplementation::addTouchSensor", "Configuring touch sensor on port " + QString::number(port));
	sensorImplementations::UnrealTouchSensorImplementation *sensor = new sensorImplementations::UnrealTouchSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	Tracer::debug(tracer::initialization, "UnrealRobotModelImplementation::addSonarSensor", "Configuring sonar sensor on port " + QString::number(port));
	sensorImplementations::UnrealSonarSensorImplementation *sensor = new sensorImplementations::UnrealSonarSensorImplementation(port, mD2Model);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void UnrealRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType)
{
	Q_UNUSED(mode)
	Tracer::debug(tracer::initialization, "UnrealRobotModelImplementation::addColorSensor", "Configuring color sensor on port " + QString::number(port));
	sensorImplementations::UnrealColorSensorImplementation *sensor = new sensorImplementations::UnrealColorSensorImplementation(port, mD2Model, sensorType);
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
	Tracer::debug(tracer::initialization, "UnrealRobotModelImplementation::timerTimeout", "Configuring sensors");
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
	mSensorsConfigurer.unlockConfiguring();
}

void UnrealRobotModelImplementation::sensorConfigurationDoneSlot()
{
	Tracer::debug(tracer::initialization, "UnrealRobotModelImplementation::sensorConfigurationDoneSlot", "");
	if (!mIsConnected) {
		mIsConnected = true;
		emit connected(true);

	} else {
		emit sensorsConfigured();
	}
	emit sensorsConfigured();
}

void UnrealRobotModelImplementation::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
	mD2Model->stopRobot();
	mIsConnected = false;
	emit disconnected();
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
	mD2Model->showModelWidget();
}
