#pragma once
#include <QtCore/QObject>
#include <QtCore/QVector>

#include "brick.h"
#include "motor.h"
#include "sensor.h"
#include "touchSensor.h"
#include "sonarSensor.h"
#include "colorSensor.h"
#include "../../sensorConstants.h"
#include "../robotImplementations/abstractRobotModelImplementation.h"

#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
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
	void clear();
	void stopRobot();

	void configureSensors(sensorType::SensorTypeEnum const &port1
			, sensorType::SensorTypeEnum const &port2
			, sensorType::SensorTypeEnum const &port3
			, sensorType::SensorTypeEnum const &port4);

	robotParts::Brick &brick();
	robotParts::TouchSensor *touchSensor(inputPort::InputPortEnum const &port) const;
	robotParts::SonarSensor *sonarSensor(inputPort::InputPortEnum const &port) const;
	robotParts::Sensor *sensor(inputPort::InputPortEnum const &port) const;
	robotParts::ColorSensor *colorSensor(inputPort::InputPortEnum const &port) const;

	robotParts::Motor &motorA();
	robotParts::Motor &motorB();
	robotParts::Motor &motorC();
	robotImplementations::AbstractRobotModelImplementation &robotImpl();
	void setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl);

private:
	robotImplementations::AbstractRobotModelImplementation *mRobotImpl;
	robotParts::Brick mBrick;
	robotParts::Motor mMotorA;
	robotParts::Motor mMotorB;
	robotParts::Motor mMotorC;

	void configureSensor(sensorType::SensorTypeEnum const &sensorType
			, inputPort::InputPortEnum const &port);
};

}
}
}
}
