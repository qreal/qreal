#pragma once
#include <QtCore/QTimer>
#include "abstractRobotModelImplementation.h"
#include "brickImplementations/unrealBrickImplementation.h"
#include "motorImplementations/unrealMotorImplementation.h"
#include "displayImplementations/unrealDisplayImplementation.h"
#include "sensorImplementations/unrealTouchSensorImplementation.h"
#include "sensorImplementations/unrealSonarSensorImplementation.h"
#include "sensorImplementations/unrealColorSensorImplementation.h"
#include "sensorImplementations/unrealLightSensorImplementation.h"
#include "sensorImplementations/unrealEncoderImplementation.h"
#include "sensorImplementations/unrealSoundSensorImplementation.h"
#include "sensorImplementations/unrealGyroscopeSensorImplementation.h"
#include "sensorImplementations/unrealAccelerometerSensorImplementation.h"
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

	virtual sensorImplementations::UnrealTouchSensorImplementation *touchSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealSonarSensorImplementation *sonarSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealColorSensorImplementation *colorSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealLightSensorImplementation *lightSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealSoundSensorImplementation *soundSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealAccelerometerSensorImplementation *accelerometerSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual sensorImplementations::UnrealGyroscopeSensorImplementation *gyroscopeSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	virtual displayImplementations::UnrealDisplayImplementation &display();

	virtual motorImplementations::UnrealMotorImplementation &motorA();
	virtual motorImplementations::UnrealMotorImplementation &motorB();
	virtual motorImplementations::UnrealMotorImplementation &motorC();

	virtual sensorImplementations::UnrealEncoderImplementation &encoderA();
	virtual sensorImplementations::UnrealEncoderImplementation &encoderB();
	virtual sensorImplementations::UnrealEncoderImplementation &encoderC();

	TimelineInterface *timeline() override;

	virtual void startInterpretation();

private slots:
	void timerTimeout();
	void sensorConfigurationDoneSlot();

private:
	QTimer mActiveWaitingTimer;
	d2Model::D2RobotModel *mD2Model;
	brickImplementations::UnrealBrickImplementation mBrick;
	motorImplementations::UnrealMotorImplementation mMotorA;
	motorImplementations::UnrealMotorImplementation mMotorB;
	motorImplementations::UnrealMotorImplementation mMotorC;

	displayImplementations::UnrealDisplayImplementation mDisplay;
	sensorImplementations::UnrealEncoderImplementation mEncoderA;
	sensorImplementations::UnrealEncoderImplementation mEncoderB;
	sensorImplementations::UnrealEncoderImplementation mEncoderC;

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
