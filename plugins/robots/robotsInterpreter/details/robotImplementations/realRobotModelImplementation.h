#pragma once

#include "abstractRobotModelImplementation.h"
#include "../robotCommunication/robotCommunicator.h"
#include "brickImplementations/realBrickImplementation.h"
#include "motorImplementations/realMotorImplementation.h"
#include "sensorImplementations/bluetoothTouchSensorImplementation.h"
#include "displayImplementations/realDisplayImplementation.h"
#include "sensorImplementations/bluetoothSonarSensorImplementation.h"
#include "sensorImplementations/bluetoothColorSensorImplementation.h"
#include "sensorImplementations/bluetoothLightSensorImplementation.h"
#include "sensorImplementations/bluetoothEncoderImplementation.h"
#include "sensorImplementations/bluetoothSoundSensorImplementation.h"
#include "sensorImplementations/bluetoothAccelerometerSensorImplementation.h"
#include "sensorImplementations/bluetoothGyroscopeSensorImplementation.h"
#include "details/realTimeline.h"

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
	virtual displayImplementations::RealDisplayImplementation &display();

	virtual sensorImplementations::BluetoothTouchSensorImplementation *touchSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothSonarSensorImplementation *sonarSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothColorSensorImplementation *colorSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothLightSensorImplementation *lightSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothSoundSensorImplementation *soundSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothAccelerometerSensorImplementation *accelerometerSensor(robots::enums::inputPort::InputPortEnum const port) const;
	virtual sensorImplementations::BluetoothGyroscopeSensorImplementation *gyroscopeSensor(robots::enums::inputPort::InputPortEnum const port) const;

	virtual motorImplementations::RealMotorImplementation &motorA();
	virtual motorImplementations::RealMotorImplementation &motorB();
	virtual motorImplementations::RealMotorImplementation &motorC();

	virtual sensorImplementations::BluetoothEncoderImplementation &encoderA();
	virtual sensorImplementations::BluetoothEncoderImplementation &encoderB();
	virtual sensorImplementations::BluetoothEncoderImplementation &encoderC();

	TimelineInterface *timeline() override;

	bool needsConnection() const;

private slots:
	void connectedSlot(bool success);
	void sensorConfigurationDoneSlot();
	void disconnectedSlot();

private:
	RobotCommunicator * const mRobotCommunicator;
	brickImplementations::RealBrickImplementation mBrick;
	displayImplementations::RealDisplayImplementation mDisplay;
	motorImplementations::RealMotorImplementation mMotorA;
	motorImplementations::RealMotorImplementation mMotorB;
	motorImplementations::RealMotorImplementation mMotorC;

	sensorImplementations::BluetoothEncoderImplementation mEncoderA;
	sensorImplementations::BluetoothEncoderImplementation mEncoderB;
	sensorImplementations::BluetoothEncoderImplementation mEncoderC;

	RealTimeline mTimeline;

	virtual void addTouchSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addSonarSensor(robots::enums::inputPort::InputPortEnum const port);

	virtual void addColorSensor(
			robots::enums::inputPort::InputPortEnum const port
			, enums::lowLevelSensorType::SensorTypeEnum mode
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			);

	virtual void addLightSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addSoundSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addAccelerometerSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addGyroscopeSensor(robots::enums::inputPort::InputPortEnum const port);

	void configureSensorImpl(robots::enums::inputPort::InputPortEnum const port);
};

}
}
}
}
}
