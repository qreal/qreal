#include "unrealRobotModelImplementation.h"
using namespace qReal::interpreters::robots;
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
	return dynamic_cast<sensorImplementations::UnrealTouchSensorImplementation *>(mSensors[port]);
}

sensorImplementations::UnrealSonarSensorImplementation *UnrealRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::UnrealSonarSensorImplementation *>(mSensors[port]);
}

sensorImplementations::UnrealColorSensorImplementation *UnrealRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::UnrealColorSensorImplementation *>(mSensors[port]);
}

void UnrealRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = new sensorImplementations::UnrealTouchSensorImplementation(port, mD2Model);
}

void UnrealRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = new sensorImplementations::UnrealSonarSensorImplementation(port, mD2Model);
}

void UnrealRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode)
{
	Q_UNUSED(mode)
	mSensors[port] = new sensorImplementations::UnrealColorSensorImplementation(port, mD2Model);
}

void UnrealRobotModelImplementation::init()
{
	AbstractRobotModelImplementation::init();
	mActiveWaitingTimer.start();
	mD2Model->startInit();
}

void UnrealRobotModelImplementation::timerTimeout()
{
	emit connected(true);
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
	mD2Model->showModelWidget();
}
