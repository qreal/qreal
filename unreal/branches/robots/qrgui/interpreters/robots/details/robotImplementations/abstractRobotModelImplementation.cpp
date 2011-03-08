#include "abstractRobotModelImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations;

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
