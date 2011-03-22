#pragma once
#include <QtCore/QTimer>
#include "abstractRobotModelImplementation.h"
#include "brickImplementations/unrealBrickImplementation.h"
#include "motorImplementations/unrealMotorImplementation.h"
#include "sensorImplementations/unrealTouchSensorImplementation.h"
#include "sensorImplementations/unrealSonarSensorImplementation.h"
#include "sensorImplementations/unrealColorSensorImplementation.h"
#include "../d2RobotModel/d2RobotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class UnrealRobotModelImplementation : public AbstractRobotModelImplementation
{
	Q_OBJECT
public:
	UnrealRobotModelImplementation(d2Model::D2RobotModel *d2RobotModel);
	virtual ~UnrealRobotModelImplementation();
	virtual void init();
	virtual void stopRobot();

	virtual brickImplementations::UnrealBrickImplementation &brick();
	virtual sensorImplementations::UnrealTouchSensorImplementation *touchSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::UnrealSonarSensorImplementation *sonarSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::UnrealColorSensorImplementation *colorSensor(inputPort::InputPortEnum const &port) const;

	virtual motorImplementations::UnrealMotorImplementation &motorA();
	virtual motorImplementations::UnrealMotorImplementation &motorB();
	virtual motorImplementations::UnrealMotorImplementation &motorC();

private slots:
	void timerTimeout();

private:
	QTimer mActiveWaitingTimer;
	d2Model::D2RobotModel *mD2Model;
	brickImplementations::UnrealBrickImplementation mBrick;
	motorImplementations::UnrealMotorImplementation mMotorA;
	motorImplementations::UnrealMotorImplementation mMotorB;
	motorImplementations::UnrealMotorImplementation mMotorC;
	virtual void addTouchSensor(inputPort::InputPortEnum const &port);
	virtual void addSonarSensor(inputPort::InputPortEnum const &port);
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode);
};

}
}
}
}
}
