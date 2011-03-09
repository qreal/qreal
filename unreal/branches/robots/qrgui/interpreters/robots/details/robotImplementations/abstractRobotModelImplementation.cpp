#include "abstractRobotModelImplementation.h"
#include "nullRobotModelImplementation.h"
#include "realRobotModelImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

NullRobotModelImplementation *AbstractRobotModelImplementation::mNullRobotModel = NULL;
RealRobotModelImplementation *AbstractRobotModelImplementation::mRealRobotModel = NULL;

AbstractRobotModelImplementation::AbstractRobotModelImplementation()
	: mSensorsToConfigure(0)
{
	mSensors.resize(4);
}

AbstractRobotModelImplementation::~AbstractRobotModelImplementation()
{
	foreach (sensorImplementations::AbstractSensorImplementation *sensor, mSensors) {
		delete sensor;
	}
}

NullRobotModelImplementation *AbstractRobotModelImplementation::nullRobotModel()
{
	if (mNullRobotModel == NULL)
		mNullRobotModel = new NullRobotModelImplementation();
	return mNullRobotModel;
}

RealRobotModelImplementation *AbstractRobotModelImplementation::realRobotModel(RobotCommunicationInterface * const robotCommunicationInterface)
{
	if (mRealRobotModel == NULL)
		mRealRobotModel = new RealRobotModelImplementation(robotCommunicationInterface);
	return mRealRobotModel;
}

AbstractRobotModelImplementation *AbstractRobotModelImplementation::robotModel(robotModelType::robotModelTypeEnum type, RobotCommunicationInterface * const robotCommunicationInterface)
{
	if (type == robotModelType::null)
		return nullRobotModel();
	else if (type == robotModelType::real)
		return realRobotModel(robotCommunicationInterface);
}

QVector<sensorImplementations::AbstractSensorImplementation *> AbstractRobotModelImplementation::sensors()
{
	return mSensors;
}

void AbstractRobotModelImplementation::init()
{
	qDebug() << "Initializing robot model...";
	qDebug() << "Connecting to robot...";
}

void AbstractRobotModelImplementation::clear()
{
}

void AbstractRobotModelImplementation::configureSensor(sensorType::SensorTypeEnum const &sensorType
		, inputPort::InputPortEnum const &port)
{
	delete mSensors[port];
	mSensors[port] = NULL;
	switch (sensorType) {
	case sensorType::unused:
		break;
	case sensorType::touchBoolean:
		addTouchSensor(port);
		break;
	case sensorType::touchRaw:
		break;
	case sensorType::sonar:
		addSonarSensor(port);
		break;
	default:
		// TODO: Throw an exception
		break;
	}
}
