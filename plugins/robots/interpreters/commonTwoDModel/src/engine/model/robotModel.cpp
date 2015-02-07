#include "robotModel.h"

#include <qmath.h>
#include <QtGui/QTransform>

#include <qrutils/mathUtils/math.h>

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

#include "constants.h"
#include "settings.h"
#include "timeline.h"

#include "physics/simplePhysicsEngine.h"
#include "physics/realisticPhysicsEngine.h"

#include "include/commonTwoDModel/robotModel/nullTwoDRobotModel.h"

using namespace twoDModel::model;
using namespace physics;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

RobotModel::RobotModel(robotModel::TwoDRobotModel &robotModel
		, const Settings &settings
		, QObject *parent)
	: QObject(parent)
	, mSettings(settings)
	, mRobotModel(robotModel)
	, mSensorsConfiguration(robotModel.robotId())
	, mPos(QPointF(0,0))
	, mAngle(0)
	, mBeepTime(0)
	, mIsOnTheGround(true)
	, mMarker(Qt::transparent)
	, mPhysicsEngine(nullptr)
{
	reinit();
}

RobotModel::~RobotModel()
{
	delete mPhysicsEngine;
}

void RobotModel::reinit()
{
	qDeleteAll(mMotors);
	mMotors.clear();
	mMarker = Qt::transparent;

	for (const Device * const device : mRobotModel.configuration().devices()) {
		if (device->deviceInfo().isA<robotParts::Motor>()) {
			initMotor(robotWheelDiameterInPx / 2, 0, 0, device->port(), false);
		}
	}

	mBeepTime = 0;
}

void RobotModel::clear()
{
	reinit();
	setPosition(QPointF());
	setRotation(0);
}

RobotModel::Motor *RobotModel::initMotor(int radius, int speed, long unsigned int degrees
		, const PortInfo &port, bool isUsed)
{
	Motor *motor = new Motor();
	motor->radius = radius;
	motor->speed = speed;
	motor->degrees = degrees;
	motor->isUsed = isUsed;
	motor->breakMode = true;
	if (degrees == 0) {
		motor->activeTimeType = DoInf;
	} else {
		motor->activeTimeType = DoByLimit;
	}

	mMotors[port] = motor;

	/// @todo We need some mechanism to set correspondence between motors and encoders. In NXT motors and encoders are
	///       physically plugged into one port, so we can find corresponding port by name. But in TRIK encoders can be
	///       connected differently.
	for (const Device * const device : mRobotModel.configuration().devices()) {
		if (device->deviceInfo().isA<EncoderSensor>()
				&& (device->port().name() == port.name() || device->port().nameAliases().contains(port.name())))
		{
			mMotorToEncoderPortMap[port] = device->port();
			mTurnoverEngines[mMotorToEncoderPortMap[port]] = 0;
		}
	}

	return motor;
}

void RobotModel::playSound(int timeInMs)
{
	mBeepTime = qMax(mBeepTime, timeInMs);
}

void RobotModel::setNewMotor(int speed, uint degrees, const PortInfo &port, bool breakMode)
{
	mMotors[port]->speed = mathUtils::Math::truncateToInterval(-100, 100, speed);
	mMotors[port]->degrees = degrees;
	mMotors[port]->isUsed = true;
	mMotors[port]->breakMode = breakMode;
	if (degrees) {
		mMotors[port]->activeTimeType = DoByLimit;
	} else {
		mMotors[port]->activeTimeType = DoInf;
	}
}

void RobotModel::countMotorTurnover()
{
	for (Motor * const motor : mMotors) {
		const PortInfo port = mMotors.key(motor);
		const qreal degrees = Timeline::timeInterval * motor->spoiledSpeed * onePercentAngularVelocity;
		mTurnoverEngines[mMotorToEncoderPortMap[port]] += degrees;
		if (motor->isUsed && (motor->activeTimeType == DoByLimit)
				&& (mTurnoverEngines[mMotorToEncoderPortMap[port]] >= motor->degrees))
		{
			motor->speed = 0;
			motor->activeTimeType = End;
		}
	}
}

int RobotModel::readEncoder(const PortInfo &port) const
{
	return mTurnoverEngines[port];
}

void RobotModel::resetEncoder(const PortInfo &port)
{
	mTurnoverEngines[port] = 0;
}

SensorsConfiguration &RobotModel::configuration()
{
	return mSensorsConfiguration;
}

twoDModel::robotModel::TwoDRobotModel &RobotModel::info()
{
	return mRobotModel;
}

void RobotModel::stopRobot()
{
	mBeepTime = 0;
	emit playingSoundChanged(false);
	for (Motor * const engine : mMotors) {
		engine->speed = 0;
		engine->breakMode = true;
	}
}

void RobotModel::countBeep()
{
	if (mBeepTime > 0) {
		emit playingSoundChanged(true);
		mBeepTime -= Timeline::timeInterval;
	} else {
		emit playingSoundChanged(false);
	}
}

QPointF RobotModel::rotationCenter() const
{
	return QPointF(mPos.x() + robotWidth / 2, mPos.y() + robotHeight / 2);
}

QPainterPath RobotModel::robotBoundingPath() const
{
	QPainterPath path;
	const QRectF boundingRect(QPointF(), QSizeF(robotWidth, robotHeight));
	path.addRect(boundingRect);

	const QPointF realRotatePoint = QPointF(boundingRect.width() / 2, boundingRect.height() / 2);
	const QPointF translationToZero = -realRotatePoint - boundingRect.topLeft();
	const QPointF finalTranslation = mPos + realRotatePoint + boundingRect.topLeft();
	const QTransform transform = QTransform().translate(finalTranslation.x(), finalTranslation.y())
			.rotate(mAngle).translate(translationToZero.x(), translationToZero.y());

	for (const PortInfo &port : mRobotModel.configurablePorts()){
		if (!mSensorsConfiguration.type(port).isNull()) {
			const QPointF sensorPos = mSensorsConfiguration.position(port);
			QPainterPath tempSensorPath;
			tempSensorPath.addRect(sensorRect(port, sensorPos));
			const QTransform transformSensor = QTransform()
					.translate(sensorPos.x(), sensorPos.y())        // /\  And going back again
					.rotate(mSensorsConfiguration.direction(port))  // ||  Then rotating
					.translate(-sensorPos.x(), -sensorPos.y());     // ||  First translating to zero
			path.addPath(transformSensor.map(tempSensorPath));
		}
	}

	return transform.map(path);
}

QRectF RobotModel::sensorRect(const PortInfo &port, const QPointF sensorPos) const
{
	if (!mSensorsConfiguration.type(port).isNull()) {
		const QSizeF size = mRobotModel.sensorImageRect(mSensorsConfiguration.type(port)).size();
		return QRectF(sensorPos - QPointF(size.width() / 2, size.height() / 2), size);
	}

	return QRectF();
}

QColor RobotModel::markerColor() const
{
	return mMarker;
}

void RobotModel::markerDown(const QColor &color)
{
	mMarker = color;
}

void RobotModel::markerUp()
{
	mMarker = Qt::transparent;
}

void RobotModel::nextStep()
{
	// Changing position quietly, they must not be caught by UI here.
	mPos += mPhysicsEngine->shift().toPointF();
	mAngle += mPhysicsEngine->rotation();
}

void RobotModel::recalculateParams()
{
	// Do nothing until robot gets back on the ground
	if (!mIsOnTheGround || !mPhysicsEngine) {
		return;
	}

	struct EngineOutput {
		qreal speed;
		bool breakMode;
	};

	auto calculateMotorOutput = [&](WheelEnum wheel) {
		const PortInfo &port = mWheelsToMotorPortsMap.value(wheel, PortInfo());
		if (!port.isValid() || port.name() == "None") {
			return EngineOutput{0, true};
		}

		Motor * const engine = mMotors.value(port, nullptr);
		if (!engine) {
			return EngineOutput{0, true};
		}

		engine->spoiledSpeed = mSettings.realisticMotors() ? varySpeed(engine->speed) : engine->speed;
		return EngineOutput{
				engine->spoiledSpeed * 2 * M_PI * engine->radius * onePercentAngularVelocity / 360
				, engine->breakMode
		};
	};

	const EngineOutput outputLeft = calculateMotorOutput(left);
	const EngineOutput outputRight = calculateMotorOutput(right);

	mPhysicsEngine->recalculateParams(Timeline::timeInterval, outputLeft.speed, outputRight.speed
			, outputLeft.breakMode, outputRight.breakMode
			, rotationCenter(), mAngle, robotBoundingPath());

	nextStep();
	countMotorTurnover();
	countBeep();
}

void RobotModel::nextFragment()
{
	if (!mIsOnTheGround) {
		return;
	}

	emit robotRided(mPos, mAngle);
}

QPointF RobotModel::position() const
{
	return mPos;
}

void RobotModel::setPosition(const QPointF &newPos)
{
	if (newPos != mPos) {
		mPos = newPos;
		emit positionChanged(newPos);
	}
}

qreal RobotModel::rotation() const
{
	return mAngle;
}

void RobotModel::setRotation(qreal angle)
{
	if (!mathUtils::Math::eq(mAngle, angle)) {
		mAngle = fmod(angle, 360);
		emit rotationChanged(angle);
	}
}

bool RobotModel::onTheGround() const
{
	return mIsOnTheGround;
}

QDomElement RobotModel::serialize(QDomDocument &target) const
{
	QDomElement robot = target.createElement("robot");
	robot.setAttribute("id", mRobotModel.robotId());
	robot.setAttribute("position", QString::number(mPos.x()) + ":" + QString::number(mPos.y()));
	robot.setAttribute("direction", mAngle);
	mSensorsConfiguration.serialize(robot, target);
	return robot;
}

void RobotModel::deserialize(const QDomElement &robotElement)
{
	const QString positionStr = robotElement.attribute("position", "0:0");
	const QStringList splittedStr = positionStr.split(":");
	const qreal x = static_cast<qreal>(splittedStr[0].toDouble());
	const qreal y = static_cast<qreal>(splittedStr[1].toDouble());
	onRobotReturnedOnGround();
	setPosition(QPointF(x, y));
	setRotation(robotElement.attribute("direction", "0").toDouble());
	nextFragment();
}

void RobotModel::onRobotLiftedFromGround()
{
	mIsOnTheGround = false;
}

void RobotModel::onRobotReturnedOnGround()
{
	mIsOnTheGround = true;
}

void RobotModel::setMotorPortOnWheel(WheelEnum wheel, const interpreterBase::robotModel::PortInfo &port)
{
	mWheelsToMotorPortsMap[wheel] = port;
}

void RobotModel::resetPhysics(const WorldModel &worldModel, const Timeline &timeline)
{
	physics::PhysicsEngineBase *oldEngine = mPhysicsEngine;
	if (mSettings.realisticPhysics()) {
		mPhysicsEngine = new physics::RealisticPhysicsEngine(worldModel, timeline);
	} else {
		mPhysicsEngine = new physics::SimplePhysicsEngine(worldModel);
	}

	delete oldEngine;
}

int RobotModel::varySpeed(const int speed) const
{
	const qreal ran = mathUtils::Math::gaussianNoise(varySpeedDispersion);
	return mathUtils::Math::truncateToInterval(-100, 100, round(speed * (1 + ran)));
}
