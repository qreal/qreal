#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "robotParts/brick.h"
#include "robotParts/motor.h"
#include "robotParts/sensor.h"
#include "robotParts/touchSensor.h"
#include "../robotCommunicationInterface.h"
#include "../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotModel : public QObject {
	Q_OBJECT

public:
	RobotModel(RobotCommunicationInterface * const robotCommunicationInterface);  // Gets ownership
	~RobotModel();

	void init();
	void clear();
	void stopRobot();

	void configureSensors(sensorType::SensorTypeEnum const &port1
			, sensorType::SensorTypeEnum const &port2
			, sensorType::SensorTypeEnum const &port3
			, sensorType::SensorTypeEnum const &port4);

	robotParts::Brick &brick();
	robotParts::TouchSensor *touchSensor(inputPort::InputPortEnum const &port) const;

	robotParts::Motor &motorA();
	robotParts::Motor &motorB();
	robotParts::Motor &motorC();

signals:
	void connected();

private slots:
	void connectedSlot();
	void sensorConfigurationDoneSlot();

private:
	RobotCommunicationInterface *mRobotCommunicationInterface;  // Has ownership
	robotParts::Brick mBrick;
	robotParts::Motor mMotorA;
	robotParts::Motor mMotorB;
	robotParts::Motor mMotorC;

	QVector<robotParts::Sensor *> mSensors;  // Has ownership
	int mSensorsToConfigure;

	void configureSensor(sensorType::SensorTypeEnum const &sensorType
			, inputPort::InputPortEnum const &port);
};

}
}
}
}
