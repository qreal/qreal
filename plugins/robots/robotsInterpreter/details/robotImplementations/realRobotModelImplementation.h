#pragma once

#include "abstractRobotModelImplementation.h"
#include "../robotCommunication/robotCommunicator.h"
#include "brickImplementations/realBrickImplementation.h"
#include "motorImplementations/realMotorImplementation.h"
#include "sensorImplementations/bluetoothTouchSensorImplementation.h"
#include "sensorImplementations/bluetoothSonarSensorImplementation.h"
#include "sensorImplementations/bluetoothColorSensorImplementation.h"
#include "sensorImplementations/bluetoothLightSensorImplementation.h"
#include "sensorImplementations/bluetoothEncoderImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class RealRobotModelImplementation : public AbstractRobotModelImplementation
{
	Q_OBJECT
public:
	RealRobotModelImplementation(RobotCommunicator * const robotCommunicationInterface);
	virtual ~RealRobotModelImplementation();
	virtual void init();
	virtual void stopRobot();
	virtual void disconnectFromRobot();

	virtual brickImplementations::RealBrickImplementation &brick();
	virtual sensorImplementations::BluetoothTouchSensorImplementation *touchSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::BluetoothSonarSensorImplementation *sonarSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::BluetoothColorSensorImplementation *colorSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::BluetoothLightSensorImplementation *lightSensor(inputPort::InputPortEnum const &port) const;

	virtual motorImplementations::RealMotorImplementation &motorA();
	virtual motorImplementations::RealMotorImplementation &motorB();
	virtual motorImplementations::RealMotorImplementation &motorC();

	virtual sensorImplementations::BluetoothEncoderImplementation &encoderA();
	virtual sensorImplementations::BluetoothEncoderImplementation &encoderB();
	virtual sensorImplementations::BluetoothEncoderImplementation &encoderC();

	bool needsConnection() const;

private slots:
	void connectedSlot(bool success);
	void sensorConfigurationDoneSlot();
	void disconnectedSlot();

private:
	RobotCommunicator * const mRobotCommunicator;
	brickImplementations::RealBrickImplementation mBrick;
	motorImplementations::RealMotorImplementation mMotorA;
	motorImplementations::RealMotorImplementation mMotorB;
	motorImplementations::RealMotorImplementation mMotorC;

	sensorImplementations::BluetoothEncoderImplementation mEncoderA;
	sensorImplementations::BluetoothEncoderImplementation mEncoderB;
	sensorImplementations::BluetoothEncoderImplementation mEncoderC;

	virtual void addTouchSensor(inputPort::InputPortEnum const &port);
	virtual void addSonarSensor(inputPort::InputPortEnum const &port);
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType);
	virtual void addLightSensor(inputPort::InputPortEnum const &port);

	void configureSensorImpl(inputPort::InputPortEnum const &port);
};

}
}
}
}
}
