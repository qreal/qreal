#include "interpreterBase/robotModel/commonRobotModel.h"

#include "interpreterBase/robotModel/configuration.h"

using namespace interpreterBase::robotModel;

//#include "../tracer.h"

//using namespace qReal::interpreters::robots;
//using namespace details;
//using namespace robotImplementations;

CommonRobotModel::CommonRobotModel()
//	: mRobotImpl(new NullRobotModelImplementation)
//	, mBrick(&mRobotImpl->brick())
//	, mDisplay(&mRobotImpl->display())
//	, mMotorA(0, &mRobotImpl->motorA())
//	, mMotorB(1, &mRobotImpl->motorA())
//	, mMotorC(2, &mRobotImpl->motorA())
//	, mEncoderA(&mRobotImpl->encoderA(), enums::outputPort::port1)
//	, mEncoderB(&mRobotImpl->encoderB(), enums::outputPort::port2)
//	, mEncoderC(&mRobotImpl->encoderC(), enums::outputPort::port3)
{
//	mSensors.resize(4);
//	connect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
//	connect(mRobotImpl, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
}

CommonRobotModel::~CommonRobotModel()
{
//	delete mRobotImpl;
//	for (int i = 0; i < 4; ++i) {
//		delete mSensors[i];
	//	}
}

QString CommonRobotModel::name() const
{
	return QString();
}

//robotParts::Brick &RobotModel::brick()
//{
//	return mBrick;
//}

//robotParts::Display &RobotModel::display()
//{
//	return mDisplay;
//}

//robotParts::TouchSensor *RobotModel::touchSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::TouchSensor *>(mSensors[port]);
//}

//robotParts::SonarSensor *RobotModel::sonarSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::SonarSensor *>(mSensors[port]);
//}

//robotParts::ColorSensor *RobotModel::colorSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::ColorSensor *>(mSensors[port]);
//}

//robotParts::LightSensor *RobotModel::lightSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::LightSensor *>(mSensors[port]);
//}

//robotParts::SoundSensor *RobotModel::soundSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::SoundSensor *>(mSensors[port]);
//}

//robotParts::GyroscopeSensor *RobotModel::gyroscopeSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::GyroscopeSensor *>(mSensors[port]);
//}

//robotParts::AccelerometerSensor *RobotModel::accelerometerSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return dynamic_cast<robotParts::AccelerometerSensor *>(mSensors[port]);
//}

//robotParts::Sensor *RobotModel::sensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return mSensors[port];
//}

//void RobotModel::configureSensors(
//		robots::enums::sensorType::SensorTypeEnum const &port1
//		, robots::enums::sensorType::SensorTypeEnum const &port2
//		, robots::enums::sensorType::SensorTypeEnum const &port3
//		, robots::enums::sensorType::SensorTypeEnum const &port4
//		)
//{
//	Tracer::debug(tracer::enums::initialization, "RobotModel::configureSensors", "Request for sensors configuration in Model");

//	if (port1 == robots::enums::sensorType::unused && port2 == robots::enums::sensorType::unused
//			&& port3 == robots::enums::sensorType::unused && port4 == robots::enums::sensorType::unused)
//	{
//		Tracer::debug(tracer::enums::initialization, "RobotModel::configureSensors", "There is no need to initialize anything, emitting and exiting");
//		emit sensorsConfigured();
//		return;
//	}

//	mRobotImpl->lockSensorsConfiguration();
//	configureSensor(port1, robots::enums::inputPort::port1);
//	configureSensor(port2, robots::enums::inputPort::port2);
//	configureSensor(port3, robots::enums::inputPort::port3);
//	configureSensor(port4, robots::enums::inputPort::port4);
//	mRobotImpl->unlockSensorsConfiguration();
//}

//void RobotModel::configureSensor(
//		robots::enums::sensorType::SensorTypeEnum const &sensorType
//		, robots::enums::inputPort::InputPortEnum const port
//		)
//{
//	mRobotImpl->configureSensor(sensorType, port);
//}

//void RobotModel::sensorsConfiguredSlot()
//{
//	Tracer::debug(tracer::enums::initialization, "RobotModel::sensorsConfiguredSlot", "Sensors configured in implementation, synching with sensors in model. Why the hell it is needed?");

//	for (int i = 0; i < 4; ++i) {
//		delete mSensors[i];  // Since it deletes a sensor that is exposed to blocks, this method can not be called when diagram is interpreted. Blocks shall be recreated after calling this one.
//		mSensors[i] = NULL;
//	}
//	for (int i = 0; i < 4; ++i) {
//		robots::enums::inputPort::InputPortEnum const port = static_cast<robots::enums::inputPort::InputPortEnum>(i);
//		sensorImplementations::AbstractSensorImplementation const * const sensorImpl = mRobotImpl->sensor(port);
//		if (sensorImpl == NULL)
//			continue;

//		robots::enums::sensorType::SensorTypeEnum const sensorType = mRobotImpl->sensor(port)->type();

//		switch (sensorType) {
//		case robots::enums::sensorType::unused:
//			break;
//		case robots::enums::sensorType::touchBoolean:
//			mSensors[port] = new robotParts::TouchSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::touchRaw:
//			break;
//		case robots::enums::sensorType::sonar:
//			mSensors[port] = new robotParts::SonarSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::colorFull:
//		case robots::enums::sensorType::colorRed:
//		case robots::enums::sensorType::colorGreen:
//		case robots::enums::sensorType::colorBlue:
//		case robots::enums::sensorType::colorNone:
//			mSensors[port] = new robotParts::ColorSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::light:
//			mSensors[port] = new robotParts::LightSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::sound:
//			mSensors[port] = new robotParts::SoundSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::gyroscope:
//			mSensors[port] = new robotParts::GyroscopeSensor(mRobotImpl->sensor(port), port);
//			break;
//		case robots::enums::sensorType::accelerometer:
//			mSensors[port] = new robotParts::AccelerometerSensor(mRobotImpl->sensor(port), port);
//			break;
//		default:
//			// TODO: Throw an exception
//			break;
//		}
//	}
//	emit sensorsConfigured();
//}

bool CommonRobotModel::needsConnection() const
{
//	return mRobotImpl->needsConnection();
	return false;
}

//void RobotModel::startInterpretation()
//{
//	return mRobotImpl->startInterpretation();
//}

//void RobotModel::nullifySensors()
//{
//	for (int port = 0; port < 4; ++port) {
//		if (mSensors[port]) {
//			mSensors[port]->nullify();
//		}
//	}
//}

//void RobotModel::connectedSlot(bool success)
//{
//	Tracer::debug(tracer::enums::initialization, "RobotModel::connectedSlot", QString("Model connection status: %1").arg(success));
//	emit connected(success);
//}

void CommonRobotModel::init()
{
	// TODO: Remove this sh~.
	emit connected(true);
//	emit sensorsConfigured();

//	Tracer::debug(tracer::enums::initialization, "RobotModel::init", "Initializing robot model");
//	mRobotImpl->init();
}

void CommonRobotModel::stopRobot()
{
//	mRobotImpl->stopRobot();
}

void CommonRobotModel::disconnectFromRobot()
{
//	mRobotImpl->disconnectFromRobot();
}

//robotParts::Motor &RobotModel::motorA()
//{
//	return mMotorA;
//}

//robotParts::Motor &RobotModel::motorB()
//{
//	return mMotorB;
//}

//robotParts::Motor &RobotModel::motorC()
//{
//	return mMotorC;
//}

//robotParts::EncoderSensor &RobotModel::encoderA()
//{
//	return mEncoderA;
//}

//robotParts::EncoderSensor &RobotModel::encoderB()
//{
//	return mEncoderB;
//}

//robotParts::EncoderSensor &RobotModel::encoderC()
//{
//	return mEncoderC;
//}

//void RobotModel::setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl)
//{
//	Tracer::debug(tracer::enums::initialization, "RobotModel::setRobotImplementation", "Setting robot implementation, current implementation is "
//			+ QString(mRobotImpl->metaObject()->className()) + ", new model implementation is "
//			+ QString(robotImpl->metaObject()->className()));

//	disconnect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
//	disconnect(mRobotImpl, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
//	mRobotImpl = robotImpl;

//	connect(robotImpl, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
//	connect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
//	connect(mRobotImpl, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));

//	mMotorA.setImplementation(&mRobotImpl->motorA());
//	mMotorB.setImplementation(&mRobotImpl->motorB());
//	mMotorC.setImplementation(&mRobotImpl->motorC());
//	mEncoderA.setImplementation(&mRobotImpl->encoderA());
//	mEncoderB.setImplementation(&mRobotImpl->encoderB());
//	mEncoderC.setImplementation(&mRobotImpl->encoderC());

//	mBrick.setImplementation(&mRobotImpl->brick());
//	mDisplay.setImplementation(&mRobotImpl->display());

//	for (int i = 0; i < 4; ++i) {
//		if (mSensors[i] != NULL) {
//			Tracer::debug(tracer::enums::initialization, "RobotModel::setRobotImplementation"
//					, "Sensor on port " + QString::number(i) + " is not null, setting implementation for it");

//			if (mRobotImpl->sensor(static_cast<robots::enums::inputPort::InputPortEnum>(i))) {
//				mSensors[i]->setImplementation(mRobotImpl->sensor(static_cast<robots::enums::inputPort::InputPortEnum>(i)));
//				Tracer::debug(tracer::enums::initialization, "RobotModel::setRobotImplementation", "Done");
//			} else {
//				delete mSensors[i];
//				mSensors[i] = NULL;
//				Tracer::debug(tracer::enums::initialization, "RobotModel::setRobotImplementation", "In current implementation it is null, sensor deleted");
//			}
//		}
//	}
//}

//void RobotModel::nextBlockAfterInitial(bool success)
//{
//	emit goToNextBlock(success);
//}

//AbstractTimer *RobotModel::produceTimer()
//{
//	return mRobotImpl->produceTimer();
//}

ConfigurationInterface &CommonRobotModel::mutableConfiguration()
{
	// TODO: Implement
	Configuration configuration;
	return configuration;
}

ConfigurationInterface const &CommonRobotModel::configuration() const
{
	// TODO: Implement
	Configuration configuration;
	return configuration;
}

robotParts::Brick &CommonRobotModel::brick()
{
	// TODO: Implement
	robotParts::Brick brick;
	return brick;
}

robotParts::Display &CommonRobotModel::display()
{
	// TODO: Implement
	robotParts::Display display;
	return display;
}

QList<PortInfo> CommonRobotModel::availablePorts() const
{
	return QList<PortInfo>();
}

QList<PluggableDeviceInfo> CommonRobotModel::supportedSensors() const
{
	return QList<PluggableDeviceInfo>();
}

