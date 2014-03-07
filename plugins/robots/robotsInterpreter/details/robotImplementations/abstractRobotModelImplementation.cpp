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
	if (mNullRobotModel == NULL) {
		mNullRobotModel = new NullRobotModelImplementation();
	}
	return mNullRobotModel;
}

RealRobotModelImplementation *AbstractRobotModelImplementation::realRobotModel(RobotCommunicator * const robotCommunicationInterface)
{
	if (mRealRobotModel == NULL) {
		mRealRobotModel = new RealRobotModelImplementation(robotCommunicationInterface);
	}
	return mRealRobotModel;
}

UnrealRobotModelImplementation *AbstractRobotModelImplementation::unrealRobotModel(d2Model::D2RobotModel *d2RobotModel)
{
	if (mUnrealRobotModel == NULL) {
		mUnrealRobotModel = new UnrealRobotModelImplementation(d2RobotModel);
	}
	return mUnrealRobotModel;
}

AbstractRobotModelImplementation *AbstractRobotModelImplementation::robotModel(
		robots::enums::robotModelType::robotModelTypeEnum type
		, RobotCommunicator * const robotCommunication
		, d2Model::D2RobotModel *d2RobotModel
		)
{
	switch (type) {
	case robots::enums::robotModelType::null:
		return nullRobotModel();
	case robots::enums::robotModelType::twoD:
		return unrealRobotModel(d2RobotModel);
	case robots::enums::robotModelType::nxt:
		return realRobotModel(robotCommunication);
	}

	// TODO: Support TRIK model.
	return nullRobotModel();
}

sensorImplementations::AbstractSensorImplementation * AbstractRobotModelImplementation::sensor(
		robots::enums::inputPort::InputPortEnum const port
		)
{
	return mSensorsConfigurer.sensor(port);
}

void AbstractRobotModelImplementation::init()
{
	Tracer::debug(tracer::enums::initialization, "AbstractRobotModelImplementation::init", "Initializing robot model and connecting to robot...");
}

void AbstractRobotModelImplementation::configureSensor(
		robots::enums::sensorType::SensorTypeEnum const &sensorType
		, robots::enums::inputPort::InputPortEnum const port
		)
{
	nullifySensor(port);
	switch (sensorType) {
	case robots::enums::sensorType::unused:
		break;
	case robots::enums::sensorType::touchBoolean:
		addTouchSensor(port);
		break;
	case robots::enums::sensorType::touchRaw:
		break;
	case robots::enums::sensorType::sonar:
		addSonarSensor(port);
		break;
	case robots::enums::sensorType::colorFull:
		addColorSensor(port, enums::lowLevelSensorType::COLORFULL, sensorType);
		break;
	case robots::enums::sensorType::colorRed:
		addColorSensor(port, enums::lowLevelSensorType::COLORRED, sensorType);
		break;
	case robots::enums::sensorType::colorGreen:
		addColorSensor(port, enums::lowLevelSensorType::COLORGREEN, sensorType);
		break;
	case robots::enums::sensorType::colorBlue:
		addColorSensor(port, enums::lowLevelSensorType::COLORBLUE, sensorType);
		break;
	case robots::enums::sensorType::colorNone:
		addColorSensor(port, enums::lowLevelSensorType::COLORNONE, sensorType);
		break;
	case robots::enums::sensorType::light:
		addLightSensor(port);
		break;
	case robots::enums::sensorType::sound:
		addSoundSensor(port);
		break;
	case robots::enums::sensorType::accelerometer:
		addAccelerometerSensor(port);
		break;
	case robots::enums::sensorType::gyroscope:
		addGyroscopeSensor(port);
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

void AbstractRobotModelImplementation::nullifySensor(robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::initialization, "AbstractRobotModelImplementation::nullifySensor", "Nullifying sensor on port " + QString::number(port));
	mSensorsConfigurer.nullifySensor(port);
}
