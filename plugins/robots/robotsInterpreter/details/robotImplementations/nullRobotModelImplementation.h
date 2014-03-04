#pragma once

#include <QtCore/QTimer>

#include "abstractRobotModelImplementation.h"
#include "brickImplementations/nullBrickImplementation.h"
#include "displayImplementations/nullDisplayImplementation.h"
#include "motorImplementations/nullMotorImplementation.h"
#include "sensorImplementations/nullTouchSensorImplementation.h"
#include "sensorImplementations/nullSonarSensorImplementation.h"
#include "sensorImplementations/nullColorSensorImplementation.h"
#include "sensorImplementations/nullLightSensorImplementation.h"
#include "sensorImplementations/nullSoundSensorImplementation.h"
#include "sensorImplementations/nullEncoderImplementation.h"
#include "sensorImplementations/nullGyroscopeSensorImplementation.h"
#include "sensorImplementations/nullAccelerometerSensorImplementation.h"
#include "details/realTimeline.h"

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
	virtual displayImplementations::NullDisplayImplementation &display();

	virtual sensorImplementations::NullTouchSensorImplementation *touchSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullSonarSensorImplementation *sonarSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullColorSensorImplementation *colorSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullLightSensorImplementation *lightSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullSoundSensorImplementation *soundSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullAccelerometerSensorImplementation *accelerometerSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::NullGyroscopeSensorImplementation *gyroscopeSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual motorImplementations::NullMotorImplementation &motorA();
	virtual motorImplementations::NullMotorImplementation &motorB();
	virtual motorImplementations::NullMotorImplementation &motorC();

	virtual sensorImplementations::NullEncoderImplementation &encoderA();
	virtual sensorImplementations::NullEncoderImplementation &encoderB();
	virtual sensorImplementations::NullEncoderImplementation &encoderC();

	TimelineInterface *timeline() override;

private slots:
	void timerTimeout();
	void sensorConfigurationDoneSlot();

private:
	QTimer mActiveWaitingTimer;
	brickImplementations::NullBrickImplementation mBrick;
	displayImplementations::NullDisplayImplementation mDisplay;
	motorImplementations::NullMotorImplementation mMotorA;
	motorImplementations::NullMotorImplementation mMotorB;
	motorImplementations::NullMotorImplementation mMotorC;

	sensorImplementations::NullEncoderImplementation mEncoderA;
	sensorImplementations::NullEncoderImplementation mEncoderB;
	sensorImplementations::NullEncoderImplementation mEncoderC;

	RealTimeline mTimeline;

	virtual void addTouchSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addSonarSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addLightSensor(robots::enums::inputPort::InputPortEnum const port);

	virtual void addColorSensor(
			robots::enums::inputPort::InputPortEnum const port
			, enums::lowLevelSensorType::SensorTypeEnum mode
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			);

	virtual void addSoundSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addGyroscopeSensor(robots::enums::inputPort::InputPortEnum const port);
	virtual void addAccelerometerSensor(robots::enums::inputPort::InputPortEnum const port);
};

}
}
}
}
}
