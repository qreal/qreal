#include "robotModel.h"

#include <qrutils/mathUtils/math.h>

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

#include "constants.h"
#include "timeline.h"

using namespace twoDModel::model;
using namespace physics;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

RobotModel::RobotModel(interpreterBase::robotModel::RobotModelInterface &robotModel
		, QObject *parent)
	: QObject(parent)
	, mSensorsConfiguration(robotModel.name())
	, mNeedSync(false)
	, mPos(QPointF(0,0))
	, mAngle(0)
	, mRobotModel(robotModel)
{
	initPosition();

//	createModelWidget();
}

void RobotModel::initPosition()
{
	qDeleteAll(mMotors);
	mMotors.clear();

	for (Device const * const device : mRobotModel.configuration().devices()) {
		if (device->deviceInfo().isA<robotParts::Motor>()) {
			initEngine(robotWheelDiameterInPx / 2, 0, 0, device->port(), false);
		}
	}

	mBeepTime = 0;
//	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
}

void RobotModel::clear()
{
	initPosition();
	mAngle = 0;
	mPos = QPointF(0,0);
}

RobotModel::Motor *RobotModel::initEngine(int radius, int speed, long unsigned int degrees
		, PortInfo const &port, bool isUsed)
{
	Motor *engine = new Motor();
	engine->radius = radius;
	engine->speed = speed;
	engine->degrees = degrees;
	engine->isUsed = isUsed;
	engine->breakMode = true;
	if (degrees == 0) {
		engine->activeTimeType = DoInf;
	} else {
		engine->activeTimeType = DoByLimit;
	}

	mMotors[port] = engine;

	/// @todo We need some mechanism to set correspondence between motors and encoders. In NXT motors and encoders are
	///       physically plugged into one port, so we can find corresponding port by name. But in TRIK encoders can be
	///       connected differently.
	for (Device const * const device : mRobotModel.configuration().devices()) {
		if (device->deviceInfo().isA<EncoderSensor>() &&
				(device->port().name() == port.name() || device->port().nameAliases().contains(port.name())))
		{
			mMotorToEncoderPortMap[port] = device->port();
			mTurnoverEngines[mMotorToEncoderPortMap[port]] = 0;
		}
	}

	return engine;
}

void RobotModel::playSound(int timeInMs)
{
	mBeepTime = qMax(mBeepTime, timeInMs);
}

void RobotModel::setNewMotor(int speed, uint degrees, PortInfo const &port, bool breakMode)
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
		PortInfo const port = mMotors.key(motor);
		qreal const degrees = Timeline::timeInterval * motor->spoiledSpeed * onePercentAngularVelocity;
		mTurnoverEngines[mMotorToEncoderPortMap[port]] += degrees;
		if (motor->isUsed && (motor->activeTimeType == DoByLimit)
				&& (mTurnoverEngines[mMotorToEncoderPortMap[port]] >= motor->degrees))
		{
			motor->speed = 0;
			motor->activeTimeType = End;
		}
	}
}

int RobotModel::readEncoder(PortInfo const &port) const
{
	return mTurnoverEngines[port];
}

void RobotModel::resetEncoder(PortInfo const &port)
{
	mTurnoverEngines[port] = 0;
}

SensorsConfiguration &RobotModel::configuration()
{
	return mSensorsConfiguration;
}

RobotModelInterface &RobotModel::info()
{
	return mRobotModel;
}

//void RobotModel::startInit()
//{
//	initPosition();
//	mTimeline->start();
//}

//void RobotModel::startInterpretation()
//{
//	startInit();
////	mD2ModelWidget->startTimelineListening();
//}

void RobotModel::stopRobot()
{
	for (Motor * const engine : mMotors) {
		engine->speed = 0;
		engine->breakMode = true;
	}

//	mDisplay->clear();
//	mD2ModelWidget->stopTimelineListening();
}

void RobotModel::countBeep()
{
	if (mBeepTime > 0) {
//		mD2ModelWidget->drawBeep(true);
		mBeepTime -= Timeline::frameLength;
	} else {
//		mD2ModelWidget->drawBeep(false);
	}
}

QPointF RobotModel::rotationCenter() const
{
	return QPointF(mPos.x() + robotWidth / 2, mPos.y() + robotHeight / 2);
}

void RobotModel::nextStep()
{
//	mPos += mPhysicsEngine->shift().toPointF();
//	mAngle += mPhysicsEngine->rotation();
}

void RobotModel::recalculateParams()
{
	// do nothing until robot gets back on the ground
//	if (!mD2ModelWidget->isRobotOnTheGround() || !mPhysicsEngine) {
//		mNeedSync = true;
//		return;
//	}

	synchronizePositions();

	struct EngineOutput {
		qreal speed;
		bool breakMode;
	};

	auto calculateMotorOutput = [&](WheelEnum wheel) {
		PortInfo const &port = mWheelsToMotorPortsMap.value(wheel, PortInfo());
		if (!port.isValid() || port.name() == "None") {
			return EngineOutput{0, true};
		}

		Motor * const engine = mMotors.value(port, nullptr);
		if (!engine) {
			return EngineOutput{0, true};
		}

//		engine->spoiledSpeed = mRealisticMotor ? varySpeed(engine->speed) : engine->speed;
//		return EngineOutput{
//				engine->spoiledSpeed * 2 * M_PI * engine->radius * onePercentAngularVelocity / 360
//				, engine->breakMode
//				};
	};

	EngineOutput const outputLeft = calculateMotorOutput(left);
	EngineOutput const outputRight = calculateMotorOutput(right);

//	mPhysicsEngine->recalculateParams(Timeline::timeInterval, outputLeft.speed, outputRight.speed
//			, outputLeft.breakMode, outputRight.breakMode
//			, rotationCenter(), mAngle
//			, mD2ModelWidget->robotBoundingPolygon(mPos, mAngle));

	nextStep();
	countMotorTurnover();
}

void RobotModel::nextFragment()
{
//	if (!mD2ModelWidget->isRobotOnTheGround()) {
//		return;
//	}

	synchronizePositions();
	countBeep();
//	mD2ModelWidget->draw(mPos, mAngle);
	mNeedSync = true;
}

void RobotModel::synchronizePositions()
{
	if (mNeedSync) {
//		mPos = mD2ModelWidget->robotPos();
		mNeedSync = false;
	}
}

void RobotModel::setRotation(qreal angle)
{
//	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
//	mAngle = fmod(angle, 360);
//	mD2ModelWidget->draw(mPos, mAngle);
}

qreal RobotModel::rotateAngle() const
{
	return mAngle;
}

void RobotModel::setRobotPos(QPointF const &newPos)
{
	mPos = newPos;
//	mD2ModelWidget->draw(mPos, mAngle);
}

QPointF RobotModel::robotPos() const
{
	return mPos;
}

void RobotModel::serialize(QDomDocument &target) const
{
	/// @todo
//	mNeedSync = true;
//	synchronizePositions();
	QDomElement robot = target.createElement("robot");
	robot.setAttribute("position", QString::number(mPos.x()) + ":" + QString::number(mPos.y()));
	robot.setAttribute("direction", mAngle);
	mSensorsConfiguration.serialize(robot, target);
	target.firstChildElement("root").appendChild(robot);
}

void RobotModel::deserialize(QDomElement const &robotElement)
{
	QString const positionStr = robotElement.attribute("position", "0:0");
	QStringList const splittedStr = positionStr.split(":");
	qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
	qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
	mPos = QPointF(x, y);
	mAngle = robotElement.attribute("direction", "0").toDouble();
	configuration().deserialize(robotElement);
	mNeedSync = false;
	nextFragment();
}

void RobotModel::setMotorPortOnWheel(WheelEnum wheel, interpreterBase::robotModel::PortInfo const &port)
{
	mWheelsToMotorPortsMap[wheel] = port;
}
