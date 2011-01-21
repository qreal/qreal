#pragma once

#include <QtCore/QObject>

#include "robotParts/brick.h"
#include "robotParts/touchSensor.h"
#include "robotParts/motor.h"
#include "../robotCommunicationInterface.h"

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

	robotParts::Brick &brick();
	robotParts::TouchSensor &touchSensor();

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
	robotParts::TouchSensor mTouchSensor;
	robotParts::Motor mMotorA;
	robotParts::Motor mMotorB;
	robotParts::Motor mMotorC;
};

}
}
}
}
