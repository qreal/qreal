#pragma once
#include <QtCore/QObject>
#include <QtCore/QVector>
#include "brickImplementations/abstractBrickImplementation.h"
#include "motorImplementations/abstractMotorImplementation.h"
#include "sensorImplementations/abstractSensorImplementation.h"
#include "../../sensorConstants.h"
#include "../../robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class NullRobotModelImplementation;
class RealRobotModelImplementation;

class AbstractRobotModelImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractRobotModelImplementation();
	virtual ~AbstractRobotModelImplementation();

	static AbstractRobotModelImplementation *robotModel(robotModelType::robotModelTypeEnum type, RobotCommunicationInterface * const robotCommunicationInterface = NULL);

	virtual void init();
	virtual void clear();
	virtual void stopRobot() = 0;

	virtual brickImplementations::AbstractBrickImplementation &brick() = 0;
	virtual sensorImplementations::AbstractSensorImplementation *touchSensor(inputPort::InputPortEnum const &port) const = 0;
	virtual sensorImplementations::AbstractSensorImplementation *sonarSensor(inputPort::InputPortEnum const &port) const = 0;

	virtual motorImplementations::AbstractMotorImplementation &motorA() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorB() = 0;
	virtual motorImplementations::AbstractMotorImplementation &motorC() = 0;

	virtual void configureSensor(sensorType::SensorTypeEnum const &sensorType
			, inputPort::InputPortEnum const &port);
	virtual QVector<sensorImplementations::AbstractSensorImplementation *> sensors();
signals:
	void connected(bool success);
protected:
	int mSensorsToConfigure;
	static NullRobotModelImplementation *mNullRobotModel;
	static RealRobotModelImplementation *mRealRobotModel;
	QVector<sensorImplementations::AbstractSensorImplementation *> mSensors;

	virtual void addTouchSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addSonarSensor(inputPort::InputPortEnum const &port) = 0;
	virtual void addColorSensor(inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode) = 0;

	static NullRobotModelImplementation *nullRobotModel();
	static RealRobotModelImplementation *realRobotModel(RobotCommunicationInterface * const robotCommunicationInterface);
};

}
}
}
}
}

