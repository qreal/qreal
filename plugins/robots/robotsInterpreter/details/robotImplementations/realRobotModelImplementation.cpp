#include "realRobotModelImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

RealRobotModelImplementation::RealRobotModelImplementation(RobotCommunication * const robotCommunicationInterface)
	:  AbstractRobotModelImplementation()
	, mRobotCommunicationInterface(robotCommunicationInterface)
	, mBrick(robotCommunicationInterface)
	, mMotorA(0, robotCommunicationInterface), mMotorB(1, robotCommunicationInterface), mMotorC(2, robotCommunicationInterface)
	, mEncoderA(robotCommunicationInterface, outputPort::port1), mEncoderB(robotCommunicationInterface, outputPort::port2), mEncoderC(robotCommunicationInterface, outputPort::port3)
{
	connect(mRobotCommunicationInterface, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	connect(mRobotCommunicationInterface, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
}

RealRobotModelImplementation::~RealRobotModelImplementation()
{
	delete mRobotCommunicationInterface;
}

brickImplementations::RealBrickImplementation &RealRobotModelImplementation::brick()
{
	return mBrick;
}

sensorImplementations::BluetoothTouchSensorImplementation *RealRobotModelImplementation::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothTouchSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::BluetoothSonarSensorImplementation *RealRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothSonarSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

sensorImplementations::BluetoothColorSensorImplementation *RealRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothColorSensorImplementation *>(mSensorsConfigurer.sensor(port));
}

void RealRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
	sensorImplementations::BluetoothTouchSensorImplementation *sensor = new sensorImplementations::BluetoothTouchSensorImplementation(mRobotCommunicationInterface, port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void RealRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	sensorImplementations::BluetoothSonarSensorImplementation *sensor = new sensorImplementations::BluetoothSonarSensorImplementation(mRobotCommunicationInterface, port);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void RealRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType)
{
	sensorImplementations::BluetoothColorSensorImplementation *sensor = new sensorImplementations::BluetoothColorSensorImplementation(mRobotCommunicationInterface, port, mode, sensorType);
	mSensorsConfigurer.configureSensor(sensor, port);
}

void RealRobotModelImplementation::init()
{
	mSensorsConfigurer.lockConfiguring();
	AbstractRobotModelImplementation::init();
	mRobotCommunicationInterface->connect();
}

void RealRobotModelImplementation::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
	for (unsigned i = 0; i < 4; ++i)
		if (colorSensor(static_cast<inputPort::InputPortEnum>(i)) != NULL)
			colorSensor(static_cast<inputPort::InputPortEnum>(i))->reconfigure(lowLevelSensorType::COLORNONE);
}

void RealRobotModelImplementation::connectedSlot(bool success)
{
	if (!success) {
		qDebug() << "Connection failed.";
		emit connected(false);
		return;
	}
	qDebug() << "Connected. Initializing sensors...";
	connect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
	mSensorsConfigurer.unlockConfiguring();
}

void RealRobotModelImplementation::sensorConfigurationDoneSlot()
{
	disconnect(&mSensorsConfigurer, SIGNAL(allSensorsConfigured()), this, SLOT(sensorConfigurationDoneSlot()));
	emit connected(true);
}

motorImplementations::RealMotorImplementation &RealRobotModelImplementation::motorA()
{
	return mMotorA;
}

motorImplementations::RealMotorImplementation &RealRobotModelImplementation::motorB()
{
	return mMotorB;
}

motorImplementations::RealMotorImplementation &RealRobotModelImplementation::motorC()
{
	return mMotorC;
}

sensorImplementations::BluetoothEncoderImplementation &RealRobotModelImplementation::encoderA()
{
	return mEncoderA;
}

sensorImplementations::BluetoothEncoderImplementation &RealRobotModelImplementation::encoderB()
{
	return mEncoderB;
}

sensorImplementations::BluetoothEncoderImplementation &RealRobotModelImplementation::encoderC()
{
	return mEncoderC;
}

bool RealRobotModelImplementation::needsConnection() const
{
	return true;
}

void RealRobotModelImplementation::disconnectedSlot()
{
	mSensorsConfigurer.lockConfiguring();
}
