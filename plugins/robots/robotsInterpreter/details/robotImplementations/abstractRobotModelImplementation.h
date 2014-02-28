#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "brickImplementations/abstractBrickImplementation.h"
#include "motorImplementations/abstractMotorImplementation.h"
#include "sensorImplementations/abstractSensorImplementation.h"
#include "displayImplementations/abstractDisplayImplementation.h"
#include "sensorImplementations/abstractEncoderImplementation.h"
#include "../../sensorConstants.h"
#include "../d2RobotModel/d2RobotModel.h"
#include "../robotCommunication/robotCommunicationThreadInterface.h"
#include "../robotCommunication/robotCommunicator.h"
#include "sensorsConfigurer.h"
#include "details/timelineInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class NullRobotModelImplementation;
class RealRobotModelImplementation;
class UnrealRobotModelImplementation;

class AbstractRobotModelImplementation : public QObject
{
	Q_OBJECT

public:
	AbstractRobotModelImplementation();
	virtual ~AbstractRobotModelImplementation();

	static AbstractRobotModelImplementation *robotModel(robots::enums::robotModelType::robotModelTypeEnum type
			, RobotCommunicator * const robotCommunicationInterface = NULL
			, d2Model::D2RobotModel *d2RobotModel = NULL
			);

	virtual void init();
	virtual void stopRobot() = 0;
	virtual void disconnectFromRobot();

	virtual brickImplementations::AbstractBrickImplementation &brick() = 0;
	virtual displayImplementations::AbstractDisplayImplementation &display() = 0;

	virtual sensorImplementations::AbstractSensorImplementation *touchSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *sonarSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *lightSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *colorSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *soundSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *accelerometerSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual sensorImplementations::AbstractSensorImplementation *gyroscopeSensor(
			robots::enums::inputPort::InputPortEnum const port
			) const = 0;

	virtual motorImplementations::AbstractMotorImplementation &motorA() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorB() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorC() = 0;

	virtual sensorImplementations::AbstractEncoderImplementation &encoderA() = 0;
	virtual sensorImplementations::AbstractEncoderImplementation &encoderB() = 0;
	virtual sensorImplementations::AbstractEncoderImplementation &encoderC() = 0;

	virtual TimelineInterface *timeline() = 0;

	virtual void configureSensor(
			robots::enums::sensorType::SensorTypeEnum const &sensorType
			, robots::enums::inputPort::InputPortEnum const port
			);

	virtual sensorImplementations::AbstractSensorImplementation * sensor(
			robots::enums::inputPort::InputPortEnum const port
			);

	virtual bool needsConnection() const;
	virtual void startInterpretation();

	void lockSensorsConfiguration();
	void unlockSensorsConfiguration();

signals:
	void connected(bool success);
	void sensorsConfigured();

	/// Emitted if robot is disconnected. Shall be only emitted when
	/// needsConnection() returns true for that model. If needsConnection()
	/// is false, it means that model is always connected and can't be disconnected at all
	/// (although it shall send connected(true) during initialization)
	void disconnected();

protected:
	virtual void addTouchSensor(robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void addSonarSensor(robots::enums::inputPort::InputPortEnum const port) = 0;

	virtual void addColorSensor(robots::enums::inputPort::InputPortEnum const port
			, enums::lowLevelSensorType::SensorTypeEnum mode
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			) = 0;

	virtual void addLightSensor(robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void addSoundSensor(robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void addAccelerometerSensor(robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void addGyroscopeSensor(robots::enums::inputPort::InputPortEnum const port) = 0;
	virtual void nullifySensor(robots::enums::inputPort::InputPortEnum const port);

	/// Connect to robot if connection doesn't established
	virtual void connectRobot();

	static NullRobotModelImplementation *nullRobotModel();

	static RealRobotModelImplementation *realRobotModel(
			RobotCommunicator * const robotCommunicationInterface
			);

	static UnrealRobotModelImplementation *unrealRobotModel(d2Model::D2RobotModel *d2RobotModel);

	static NullRobotModelImplementation *mNullRobotModel;
	static RealRobotModelImplementation *mRealRobotModel;
	static UnrealRobotModelImplementation *mUnrealRobotModel;
	SensorsConfigurer mSensorsConfigurer;
	bool mIsConnected;
};

}
}
}
}
}
