#pragma once
#include <QtCore/QTimer>
#include "abstractRobotModelImplementation.h"
#include "brickImplementations/nullBrickImplementation.h"
#include "motorImplementations/nullMotorImplementation.h"
#include "sensorImplementations/nullTouchSensorImplementation.h"
#include "sensorImplementations/nullSonarSensorImplementation.h"
#include "sensorImplementations/nullColorSensorImplementation.h"
#include "sensorImplementations/nullEncoderImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class NullRobotModelImplementation : public AbstractRobotModelImplementation
{
	Q_OBJECT
public:
	NullRobotModelImplementation();
	virtual ~NullRobotModelImplementation() {}
	virtual void init();
	virtual void stopRobot();

	virtual brickImplementations::NullBrickImplementation &brick();
	virtual sensorImplementations::NullTouchSensorImplementation *touchSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::NullSonarSensorImplementation *sonarSensor(inputPort::InputPortEnum const &port) const;
	virtual sensorImplementations::NullColorSensorImplementation *colorSensor(inputPort::InputPortEnum const &port) const;

	virtual motorImplementations::NullMotorImplementation &motorA();
	virtual motorImplementations::NullMotorImplementation &motorB();
	virtual motorImplementations::NullMotorImplementation &motorC();

	virtual sensorImplementations::NullEncoderImplementation &encoderA();
	virtual sensorImplementations::NullEncoderImplementation &encoderB();
	virtual sensorImplementations::NullEncoderImplementation &encoderC();

private slots:
	void timerTimeout();
	void sensorConfigurationDoneSlot();

private:
	QTimer mActiveWaitingTimer;
	brickImplementations::NullBrickImplementation mBrick;
	motorImplementations::NullMotorImplementation mMotorA;
	motorImplementations::NullMotorImplementation mMotorB;
	motorImplementations::NullMotorImplementation mMotorC;

	sensorImplementations::NullEncoderImplementation mEncoderA;
	sensorImplementations::NullEncoderImplementation mEncoderB;
	sensorImplementations::NullEncoderImplementation mEncoderC;

	virtual void addTouchSensor(inputPort::InputPortEnum const &port);
	virtual void addSonarSensor(inputPort::InputPortEnum const &port);
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType);
};

}
}
}
}
}
