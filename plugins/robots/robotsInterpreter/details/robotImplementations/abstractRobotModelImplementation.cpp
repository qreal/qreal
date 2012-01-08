#include "abstractRobotModelImplementation.h"

#include "../../../../../qrkernel/exception/exception.h"

#include "nullRobotModelImplementation.h"
#include "realRobotModelImplementation.h"
#include "unrealRobotModelImplementation.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations;

NullRobotModelImplementation *AbstractRobotModelImplementation::mNullRobotModel = NULL;
RealRobotModelImplementation *AbstractRobotModelImplementation::mRealRobotModel = NULL;
UnrealRobotModelImplementation *AbstractRobotModelImplementation::mUnrealRobotModel = NULL;

AbstractRobotModelImplementation::AbstractRobotModelImplementation()
	: mIsConnected(false)
{
	mSensorsConfigurer.lockConfiguring();  // Model is not connected yet.
}

AbstractRobotModelImplementation::~AbstractRobotModelImplementation()
{
}

NullRobotModelImplementation *AbstractRobotModelImplementation::nullRobotModel()
{
	if (mNullRobotModel == NULL)
		mNullRobotModel = new NullRobotModelImplementation();
	return mNullRobotModel;
}

RealRobotModelImplementation *AbstractRobotModelImplementation::realRobotModel(RobotCommunicator * const robotCommunicationInterface)
{
	if (mRealRobotModel == NULL)
		mRealRobotModel = new RealRobotModelImplementation(robotCommunicationInterface);
	return mRealRobotModel;
}

UnrealRobotModelImplementation *AbstractRobotModelImplementation::unrealRobotModel(d2Model::D2RobotModel *d2RobotModel)
{
	if (mUnrealRobotModel == NULL)
		mUnrealRobotModel = new UnrealRobotModelImplementation(d2RobotModel);
	return mUnrealRobotModel;
}

AbstractRobotModelImplementation *AbstractRobotModelImplementation::robotModel(robotModelType::robotModelTypeEnum type, RobotCommunicator * const robotCommunication, d2Model::D2RobotModel *d2RobotModel)
{
	if (type == robotModelType::null)
		return nullRobotModel();
	else if (type == robotModelType::unreal)
		return unrealRobotModel(d2RobotModel);
	else if (type == robotModelType::real)
		return realRobotModel(robotCommunication);
	else
		throw Exception("AbstractRobotModelImplementation::robotModel tried to create unknown robot model");
}

sensorImplementations::AbstractSensorImplementation * AbstractRobotModelImplementation::sensor(inputPort::InputPortEnum const &port)
{
	return mSensorsConfigurer.sensor(port);
}

void AbstractRobotModelImplementation::init()
{
	Tracer::debug(tracer::initialization, "AbstractRobotModelImplementation::init", "Initializing robot model and connecting to robot...");
}

void AbstractRobotModelImplementation::configureSensor(sensorType::SensorTypeEnum const &sensorType
		, inputPort::InputPortEnum const &port)
{
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
	case sensorType::colorFull:
		addColorSensor(port, lowLevelSensorType::COLORFULL, sensorType);
		break;
	case sensorType::colorRed:
		addColorSensor(port, lowLevelSensorType::COLORRED, sensorType);
		break;
	case sensorType::colorGreen:
		addColorSensor(port, lowLevelSensorType::COLORGREEN, sensorType);
		break;
	case sensorType::colorBlue:
		addColorSensor(port, lowLevelSensorType::COLORBLUE, sensorType);
		break;
	case sensorType::colorNone:
		addColorSensor(port, lowLevelSensorType::COLORNONE, sensorType);
		break;
	default:
		// TODO: Throw an exception
		break;
	}
}

bool AbstractRobotModelImplementation::needsConnection() const
{
	return false;
}

void AbstractRobotModelImplementation::startInterpretation()
{
}

void AbstractRobotModelImplementation::lockSensorsConfiguration()
{
	mSensorsConfigurer.lockConfiguring();
}

void AbstractRobotModelImplementation::unlockSensorsConfiguration()
{
	mSensorsConfigurer.unlockConfiguring();
}

void AbstractRobotModelImplementation::connectRobot()
{
	if (!mIsConnected) {
		mIsConnected = true;
		emit connected(true);
	}
	emit sensorsConfigured();
}

void AbstractRobotModelImplementation::disconnectFromRobot()
{
}
