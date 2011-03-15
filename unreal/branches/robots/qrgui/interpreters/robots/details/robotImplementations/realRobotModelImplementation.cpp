#include "realRobotModelImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

RealRobotModelImplementation::RealRobotModelImplementation(RobotCommunicationInterface * const robotCommunicationInterface)
	:  AbstractRobotModelImplementation()
	, mRobotCommunicationInterface(robotCommunicationInterface)
	, mBrick(robotCommunicationInterface)
	, mMotorA(0, robotCommunicationInterface), mMotorB(1, robotCommunicationInterface), mMotorC(2, robotCommunicationInterface)
{
	connect(mRobotCommunicationInterface, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
}

RealRobotModelImplementation::~RealRobotModelImplementation()
{
	delete mRobotCommunicationInterface;
	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mSensors)
		delete sensor;
}
brickImplementations::RealBrickImplementation &RealRobotModelImplementation::brick()
{
	return mBrick;
}

sensorImplementations::BluetoothTouchSensorImplementation *RealRobotModelImplementation::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothTouchSensorImplementation *>(mSensors[port]);
}

sensorImplementations::BluetoothSonarSensorImplementation *RealRobotModelImplementation::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothSonarSensorImplementation *>(mSensors[port]);
}

sensorImplementations::BluetoothColorSensorImplementation *RealRobotModelImplementation::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<sensorImplementations::BluetoothColorSensorImplementation *>(mSensors[port]);
}

void RealRobotModelImplementation::addTouchSensor(inputPort::InputPortEnum const &port)
{
//	lowLevelInputPort::InputPortEnum const lowLevelPort = static_cast<lowLevelInputPort::InputPortEnum>(port);
	mSensors[port] = new sensorImplementations::BluetoothTouchSensorImplementation(mRobotCommunicationInterface, port/*lowLevelPort*/);
}

void RealRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
//	lowLevelInputPort::InputPortEnum const lowLevelPort = static_cast<lowLevelInputPort::InputPortEnum>(port);
	mSensors[port] = new sensorImplementations::BluetoothSonarSensorImplementation(mRobotCommunicationInterface, port/*lowLevelPort*/);
}

void RealRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port)
{
//	lowLevelInputPort::InputPortEnum const lowLevelPort = static_cast<lowLevelInputPort::InputPortEnum>(port);
	mSensors[port] = new sensorImplementations::BluetoothColorSensorImplementation(mRobotCommunicationInterface, port/*lowLevelPort*/);
}

void RealRobotModelImplementation::init()
{
	AbstractRobotModelImplementation::init();
	mRobotCommunicationInterface->connect();
}

void RealRobotModelImplementation::stopRobot()
{
	mMotorA.off();
	mMotorB.off();
	mMotorC.off();
}

void RealRobotModelImplementation::connectedSlot(bool success)
{
	if (!success) {
		qDebug() << "Connection failed.";
		emit connected(false);
		return;
	}
	qDebug() << "Connected. Initializing sensors...";
	foreach (sensorImplementations::AbstractSensorImplementation *aSensor, mSensors) {
		sensorImplementations::BluetoothSensorImplementation *sensor = dynamic_cast<sensorImplementations::BluetoothSensorImplementation *>(aSensor);
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

void RealRobotModelImplementation::sensorConfigurationDoneSlot()
{
	--mSensorsToConfigure;
	if (mSensorsToConfigure == 0) {
		qDebug() << "Sensor configuration done";
		emit connected(true);
	}
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

