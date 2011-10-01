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
#include "../robotCommunication/robotCommunication.h"
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

	static AbstractRobotModelImplementation *robotModel(robotModelType::robotModelTypeEnum type, RobotCommunication * const robotCommunicationInterface = NULL, d2Model::D2RobotModel *d2RobotModel = NULL);

	virtual void init();
	virtual void stopRobot() = 0;

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

signals:
	void connected(bool success);

protected:
//	int mSensorsToConfigure;
	static NullRobotModelImplementation *mNullRobotModel;
	static RealRobotModelImplementation *mRealRobotModel;
	static UnrealRobotModelImplementation *mUnrealRobotModel;
//	QVector<sensorImplementations::AbstractSensorImplementation *> mSensors;
	SensorsConfigurer mSensorsConfigurer;

	virtual void addTouchSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addSonarSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode, sensorType::SensorTypeEnum const &sensorType) = 0;

	static NullRobotModelImplementation *nullRobotModel();
	static RealRobotModelImplementation *realRobotModel(RobotCommunication * const robotCommunicationInterface);
	static UnrealRobotModelImplementation *unrealRobotModel(d2Model::D2RobotModel *d2RobotModel);
};

}
}
}
}
}
