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
	delete mSensors[port];
	mSensors[port] = new sensorImplementations::BluetoothTouchSensorImplementation(mRobotCommunicationInterface, port);
}

void RealRobotModelImplementation::addSonarSensor(inputPort::InputPortEnum const &port)
{
	delete mSensors[port];
	mSensors[port] = new sensorImplementations::BluetoothSonarSensorImplementation(mRobotCommunicationInterface, port);
}

void RealRobotModelImplementation::addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode)
{
	delete mSensors[port];
	mSensors[port] = new sensorImplementations::BluetoothColorSensorImplementation(mRobotCommunicationInterface, port, mode);
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

void RealRobotModelImplementation::configureSensorImpl(inputPort::InputPortEnum const &port)
{
	sensorImplementations::AbstractSensorImplementation *aSensor = mSensors[port];
	sensorImplementations::BluetoothSensorImplementation *sensor = dynamic_cast<sensorImplementations::BluetoothSensorImplementation *>(aSensor);
	if (sensor != NULL) {
		connect(sensor, SIGNAL(configured()), this, SLOT(sensorConfigurationDoneSlot()));
		sensor->configure();
		++mSensorsToConfigure;
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
