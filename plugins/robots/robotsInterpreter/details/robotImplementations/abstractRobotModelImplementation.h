#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "brickImplementations/abstractBrickImplementation.h"
#include "motorImplementations/abstractMotorImplementation.h"
#include "sensorImplementations/abstractSensorImplementation.h"
#include "sensorImplementations/abstractEncoderImplementation.h"
#include "../../sensorConstants.h"
#include "../d2RobotModel/d2RobotModel.h"
#include "../robotCommunication/robotCommunicationThreadInterface.h"
#include "../robotCommunication/robotCommunicator.h"
#include "sensorsConfigurer.h"

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

	static AbstractRobotModelImplementation *robotModel(robotModelType::robotModelTypeEnum type, RobotCommunicator * const robotCommunicationInterface = NULL, d2Model::D2RobotModel *d2RobotModel = NULL);

	virtual void init();
	virtual void stopRobot() = 0;
	virtual void disconnectFromRobot();

	virtual brickImplementations::AbstractBrickImplementation &brick() = 0;
	virtual sensorImplementations::AbstractSensorImplementation *touchSensor(inputPort::InputPortEnum const &port) const = 0;
	virtual sensorImplementations::AbstractSensorImplementation *sonarSensor(inputPort::InputPortEnum const &port) const = 0;

	virtual motorImplementations::AbstractMotorImplementation &motorA() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorB() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorC() = 0;

	virtual sensorImplementations::AbstractEncoderImplementation &encoderA() = 0;
	virtual sensorImplementations::AbstractEncoderImplementation &encoderB() = 0;
	virtual sensorImplementations::AbstractEncoderImplementation &encoderC() = 0;

	virtual void configureSensor(sensorType::SensorTypeEnum const &sensorType
			, inputPort::InputPortEnum const &port);
	virtual sensorImplementations::AbstractSensorImplementation * sensor(inputPort::InputPortEnum const &port);

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
	virtual void addTouchSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addSonarSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType) = 0;

	/// Connect to robot if connection doesn't established
	virtual void connectRobot();

	static NullRobotModelImplementation *nullRobotModel();
	static RealRobotModelImplementation *realRobotModel(RobotCommunicator * const robotCommunicationInterface);
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
