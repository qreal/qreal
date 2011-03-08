#pragma once
#include <QtCore/QObject>
#include <QtCore/QVector>

#include "brick.h"
#include "motor.h"
#include "sensor.h"
#include "touchSensor.h"
#include "sonarSensor.h"
#include "../../sensorConstants.h"
#include "../robotImplementations/abstractRobotModelImplementation.h"

#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
#include "../robotImplementations/motorImplementations/abstractMotorImplementation.h"
#include "../robotImplementations/sensorImplementations/bluetoothTouchSensorImplementation.h"
#include "../robotImplementations/sensorImplementations/bluetoothSonarSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotModel : public QObject {
	Q_OBJECT

public:
	RobotModel(robotImplementations::AbstractRobotModelImplementation *robotImpl);
	virtual ~RobotModel();

	virtual void init();
	virtual void clear();
	virtual void stopRobot();

	virtual void configureSensors(sensorType::SensorTypeEnum const &port1
			, sensorType::SensorTypeEnum const &port2
			, sensorType::SensorTypeEnum const &port3
			, sensorType::SensorTypeEnum const &port4);

	robotParts::Brick &brick();
	robotParts::TouchSensor *touchSensor(inputPort::InputPortEnum const &port) const;
	robotParts::SonarSensor *sonarSensor(inputPort::InputPortEnum const &port) const;

	robotParts::Motor &motorA();
	robotParts::Motor &motorB();
	robotParts::Motor &motorC();
	virtual robotImplementations::AbstractRobotModelImplementation &robotImpl();

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
