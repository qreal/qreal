#pragma once

#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterBase {
namespace robotModel {

class RobotModel : public RobotModelInterface {
	Q_OBJECT

public:
	RobotModel();
	virtual ~RobotModel();

	virtual void init();
	virtual void stopRobot();
	virtual void disconnectFromRobot();

//	void configureSensors(
//			robots::enums::sensorType::SensorTypeEnum const &port1
//			, robots::enums::sensorType::SensorTypeEnum const &port2
//			, robots::enums::sensorType::SensorTypeEnum const &port3
//			, robots::enums::sensorType::SensorTypeEnum const &port4
//			);

//	robotParts::Brick &brick();
//	robotParts::Display &display();

//	robotParts::TouchSensor *touchSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	robotParts::SonarSensor *sonarSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	robotParts::LightSensor *lightSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	robotParts::Sensor *sensor(robots::enums::inputPort::InputPortEnum const port) const;
//	robotParts::ColorSensor *colorSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	robotParts::SoundSensor *soundSensor(robots::enums::inputPort::InputPortEnum const port) const;

//	robotParts::GyroscopeSensor *gyroscopeSensor(
//			robots::enums::inputPort::InputPortEnum const port
//			) const;

//	robotParts::AccelerometerSensor *accelerometerSensor(
//			robots::enums::inputPort::InputPortEnum const port
//			) const;

//	robotParts::Motor &motorA();
//	robotParts::Motor &motorB();
//	robotParts::Motor &motorC();
//	robotParts::EncoderSensor &encoderA();
//	robotParts::EncoderSensor &encoderB();
//	robotParts::EncoderSensor &encoderC();

//	robotImplementations::AbstractRobotModelImplementation &robotImpl();
//	void setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl);
	virtual bool needsConnection() const;
//	void startInterpretation();

//	void nullifySensors();

//	void nextBlockAfterInitial(bool success);

	/// Creates new timer for specific implementation. Doesn`t take ownership
//	AbstractTimer *produceTimer();

};

}
}
