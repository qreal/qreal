#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "brick.h"
#include "display.h"
#include "motor.h"
#include "sensor.h"
#include "touchSensor.h"
#include "sonarSensor.h"
#include "colorSensor.h"
#include "encoderSensor.h"
#include "lightSensor.h"
#include "soundSensor.h"
#include "gyroscopeSensor.h"
#include "accelerometerSensor.h"
#include "../../sensorConstants.h"
#include "../robotImplementations/abstractRobotModelImplementation.h"

#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
#include "../robotImplementations/displayImplementations/abstractDisplayImplementation.h"
#include "../robotImplementations/motorImplementations/abstractMotorImplementation.h"
#include "../robotImplementations/sensorImplementations/bluetoothTouchSensorImplementation.h"
#include "../robotImplementations/sensorImplementations/bluetoothSonarSensorImplementation.h"

#include "../robotImplementations/nullRobotModelImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotModel : public QObject {
	Q_OBJECT

public:
	RobotModel();
	virtual ~RobotModel();

	void init();
	void stopRobot();
	void disconnectFromRobot();

	void configureSensors(
			robots::enums::sensorType::SensorTypeEnum const &port1
			, robots::enums::sensorType::SensorTypeEnum const &port2
			, robots::enums::sensorType::SensorTypeEnum const &port3
			, robots::enums::sensorType::SensorTypeEnum const &port4
			);

	robotParts::Brick &brick();
	robotParts::Display &display();

	robotParts::TouchSensor *touchSensor(robots::enums::inputPort::InputPortEnum const port) const;
	robotParts::SonarSensor *sonarSensor(robots::enums::inputPort::InputPortEnum const port) const;
	robotParts::LightSensor *lightSensor(robots::enums::inputPort::InputPortEnum const port) const;
	robotParts::Sensor *sensor(robots::enums::inputPort::InputPortEnum const port) const;
	robotParts::ColorSensor *colorSensor(robots::enums::inputPort::InputPortEnum const port) const;
	robotParts::SoundSensor *soundSensor(robots::enums::inputPort::InputPortEnum const port) const;

	robotParts::GyroscopeSensor *gyroscopeSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	robotParts::AccelerometerSensor *accelerometerSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	robotParts::Motor &motorA();
	robotParts::Motor &motorB();
	robotParts::Motor &motorC();
	robotParts::EncoderSensor &encoderA();
	robotParts::EncoderSensor &encoderB();
	robotParts::EncoderSensor &encoderC();

	robotImplementations::AbstractRobotModelImplementation &robotImpl();
	void setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl);
	bool needsConnection() const;
	void startInterpretation();

	void nullifySensors();

	void nextBlockAfterInitial(bool success);

	TimelineInterface *timeline();

signals:
	void sensorsConfigured();
	void connected(bool success);

	/// Is emitted if robot is disconnected
	void disconnected();

	void goToNextBlock(bool success);

private slots:
	void sensorsConfiguredSlot();
	void connectedSlot(bool success);

private:
	robotImplementations::AbstractRobotModelImplementation *mRobotImpl;  // Has ownership.
	robotParts::Brick mBrick;
	robotParts::Display mDisplay;
	robotParts::Motor mMotorA;
	robotParts::Motor mMotorB;
	robotParts::Motor mMotorC;
	robotParts::EncoderSensor mEncoderA;
	robotParts::EncoderSensor mEncoderB;
	robotParts::EncoderSensor mEncoderC;

	QVector<robotParts::Sensor *> mSensors;  // Has ownership.

	void configureSensor(
			robots::enums::sensorType::SensorTypeEnum const &sensorType
			, robots::enums::inputPort::InputPortEnum const port
			);
};

}
}
}
}
