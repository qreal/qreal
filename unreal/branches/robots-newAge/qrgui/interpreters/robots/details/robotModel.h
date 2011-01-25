#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "robotParts/brick.h"
#include "robotParts/motor.h"
#include "robotParts/sensor.h"
#include "robotParts/touchSensor.h"
#include "../robotCommunicationInterface.h"
#include "../sensorType.h"

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

	void configureSensors(SensorType::SensorType const &port1, SensorType::SensorType const &port2
			, SensorType::SensorType const &port3, SensorType::SensorType const &port4);

	robotParts::Brick &brick();
	robotParts::TouchSensor *touchSensor(int port) const;

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

	void configureSensor(SensorType::SensorType const &sensorType, int port);
};

}
}
}
}
