/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "twoDModel/engine/model/robotModel.h"

#include <qmath.h>
#include <QtCore/QtMath>
#include <QtGui/QTransform>

#include <qrutils/mathUtils/math.h>

#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/motor.h>

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/settings.h"
#include "twoDModel/engine/model/timeline.h"

#include "physics/physicsEngineBase.h"

#include "src/engine/items/startPosition.h"

using namespace twoDModel::model;
using namespace kitBase::robotModel;
using namespace kitBase::robotModel::robotParts;

const int positionStampsCount = 50;

RobotModel::RobotModel(robotModel::TwoDRobotModel &robotModel
		, const Settings &settings
		, QObject *parent)
	: QObject(parent)
	, mSettings(settings)
	, mRobotModel(robotModel)
	, mSensorsConfiguration(robotModel.robotId(), robotModel.size())
	, mPos(QPointF(0,0))
	, mAngle(0)
	, mDeltaRadiansOfAngle(0)
	, mBeepTime(0)
	, mIsOnTheGround(true)
	, mMarker(Qt::transparent)
	, mAcceleration(QPointF(0, 0))
	, mPosStamps(positionStampsCount)
	, mIsFirstAngleStamp(true)
	, mAngleStampPrevious(0)
	, mPhysicsEngine(nullptr)
	, mStartPositionMarker(new items::StartPosition(info().size()))
{
	reinit();
}

RobotModel::~RobotModel()
{
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
	mDeltaRadiansOfAngle = 0;
	mAcceleration = QPointF(0, 0);
}

void RobotModel::clear()
{
	reinit();
	setPosition(QPointF());
	setRotation(0);
}

RobotModel::Wheel *RobotModel::initMotor(int radius, int speed, long unsigned int degrees
		, const PortInfo &port, bool isUsed)
{
	Wheel *motor = new Wheel();
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
	for (Wheel * const motor : mMotors) {
		const PortInfo port = mMotors.key(motor);
		const qreal degrees = Timeline::timeInterval * motor->spoiledSpeed * mRobotModel.onePercentAngularVelocity();
		//const qreal actualDegrees = mPhysicsEngine->isRobotStuck() ? 0 : degrees;
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

const RobotModel::Wheel &RobotModel::leftWheel() const
{
	return *mMotors[mWheelsToMotorPortsMap[left]];
}

const RobotModel::Wheel &RobotModel::rightWheel() const
{
	return *mMotors[mWheelsToMotorPortsMap[right]];
}

twoDModel::robotModel::TwoDRobotModel &RobotModel::info() const
{
	return mRobotModel;
}

void RobotModel::stopRobot()
{
	mBeepTime = 0;
	mRobotModel.displayWidget()->reset();
	mIsFirstAngleStamp = true;
	mPosStamps.clear();
	emit playingSoundChanged(false);
	for (Wheel * const engine : mMotors) {
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

void RobotModel::countSpeedAndAcceleration()
{
	if (mIsFirstAngleStamp) {
		mAngleStampPrevious = mAngle;
		mIsFirstAngleStamp = false;
	} else {
		mDeltaRadiansOfAngle = qDegreesToRadians(mAngle - mAngleStampPrevious);
		mAngleStampPrevious = mAngle;
	}

	if (mPosStamps.size() >= positionStampsCount) {
		mPosStamps.dequeue();
	}

	mPosStamps.enqueue(mPos);
	mAcceleration = averageAcceleration();
}

QPointF RobotModel::averageAcceleration() const
{
	/// Some arcane formula that produces natural-looking results for some reason (with correct accelerometerConstant,
	/// since mPosStamps.size() as a divisor is obviously wrong here).
	/// Maybe it will be better to actually count average.

	return mPosStamps.size() < 2 ? QPointF() : (mPosStamps.size() < 4
			? (mPosStamps.tail() - mPosStamps.head()) / mPosStamps.size()
			: (mPosStamps.tail() - mPosStamps.nthFromTail(1)
					- mPosStamps.nthFromHead(1) + mPosStamps.head()) / mPosStamps.size());
}

QPointF RobotModel::rotationCenter() const
{
	return mPos + mRobotModel.rotationCenter();
}

QPainterPath RobotModel::robotBoundingPath() const
{
	QPainterPath path;
	const QRectF boundingRect(QPointF(), mRobotModel.size());
	path.addRect(boundingRect);

	const QPointF realRotatePoint = QPointF(boundingRect.width() / 2, boundingRect.height() / 2);
	const QPointF translationToZero = -realRotatePoint - boundingRect.topLeft();
	const QPointF finalTranslation = mPos + realRotatePoint + boundingRect.topLeft();
	const QTransform transform = QTransform().translate(finalTranslation.x(), finalTranslation.y())
			.rotate(mAngle).translate(translationToZero.x(), translationToZero.y());

	for (const PortInfo &port : mRobotModel.configurablePorts()){
		if (!mSensorsConfiguration.type(port).isNull() && mSensorsConfiguration.type(port).simulated()) {
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

void RobotModel::setPhysicalEngine(physics::PhysicsEngineBase &engine)
{
	mPhysicsEngine = &engine;
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

QVector<int> RobotModel::accelerometerReading() const
{
	return {static_cast<int>(mAcceleration.x() * accelerometerConstant)
				, static_cast<int>(mAcceleration.y() * accelerometerConstant)
				, g};
}

QVector<int> RobotModel::gyroscopeReading() const
{
	return {0, 0, static_cast<int>(mDeltaRadiansOfAngle * gyroscopeConstant)};
}

void RobotModel::nextStep()
{
	// Changing position quietly, they must not be caught by UI here.
	mPos += mPhysicsEngine->positionShift(*this).toPointF();
	mAngle += mPhysicsEngine->rotation(*this);
	emit positionRecalculated(mPos, mAngle);
}

void RobotModel::recalculateParams()
{
	// Do nothing until robot gets back on the ground
	if (!mIsOnTheGround || !mPhysicsEngine) {
		return;
	}

	auto calculateMotorOutput = [&](WheelEnum wheel) {
		const PortInfo &port = mWheelsToMotorPortsMap.value(wheel, PortInfo());
		if (!port.isValid() || port.name() == "None") {
			return;
		}

		Wheel * const engine = mMotors.value(port, nullptr);
		if (!engine) {
			return;
		}

		engine->spoiledSpeed = mSettings.realisticMotors() ? varySpeed(engine->speed) : engine->speed;
	};

	calculateMotorOutput(left);
	calculateMotorOutput(right);

	nextStep();
	countSpeedAndAcceleration();
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

QDomElement RobotModel::serialize(QDomElement &parent) const
{
	QDomElement robot = parent.ownerDocument().createElement("robot");
	parent.appendChild(robot);
	robot.setAttribute("id", mRobotModel.robotId());
	robot.setAttribute("position", QString::number(mPos.x()) + ":" + QString::number(mPos.y()));
	robot.setAttribute("direction", QString::number(mAngle));

	mSensorsConfiguration.serialize(robot);
	mStartPositionMarker->serialize(robot);
	serializeWheels(robot);

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
	mStartPositionMarker->deserializeCompatibly(robotElement);
	deserializeWheels(robotElement);
	emit deserialized(QPointF(x, y), robotElement.attribute("direction", "0").toDouble());
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

void RobotModel::setMotorPortOnWheel(WheelEnum wheel, const kitBase::robotModel::PortInfo &port)
{
	if (mWheelsToMotorPortsMap[wheel] != port) {
		mWheelsToMotorPortsMap[wheel] = port;
		emit wheelOnPortChanged(wheel, port);
	}
}

int RobotModel::varySpeed(const int speed) const
{
	const qreal ran = mathUtils::Math::gaussianNoise(varySpeedDispersion);
	return mathUtils::Math::truncateToInterval(-100, 100, round(speed * (1 + ran)));
}

void RobotModel::serializeWheels(QDomElement &robotElement) const
{
	QDomElement wheels = robotElement.ownerDocument().createElement("wheels");
	wheels.setAttribute("left", mWheelsToMotorPortsMap[WheelEnum::left].toString());
	wheels.setAttribute("right", mWheelsToMotorPortsMap[WheelEnum::right].toString());
	robotElement.appendChild(wheels);
}

void RobotModel::deserializeWheels(const QDomElement &robotElement)
{
	const QDomElement wheels = robotElement.firstChildElement("wheels");
	if (wheels.isNull()) {
		return;
	}

	setMotorPortOnWheel(WheelEnum::left, PortInfo::fromString(wheels.attribute("left")));
	setMotorPortOnWheel(WheelEnum::right, PortInfo::fromString(wheels.attribute("right")));
}

QGraphicsItem *RobotModel::startPositionMarker() const
{
	return mStartPositionMarker;
}
