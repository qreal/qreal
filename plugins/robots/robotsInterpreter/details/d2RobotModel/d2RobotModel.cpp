#include "d2RobotModel.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2RobotModel::D2RobotModel(QObject *parent)
	: QObject(parent)
	, mD2ModelWidget(NULL)
{
	mAngle = 0;
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(nextFragment()));
	initPosition();
}

D2RobotModel::~D2RobotModel()
{
}

void D2RobotModel::initPosition()
{
	mMotorA = initMotor(5, 0, 0, 0);
	mMotorB = initMotor(5, 0, 0, 1);
	mMotorC = initMotor(5, 0, 0, 2);
        setBeep(0, 0);
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
	mRotatePoint  = QPointF(0, 0);

}

void D2RobotModel::clear()
{
	initPosition();
	mAngle = 0;
	mPos = QPointF(0,0);
}

D2RobotModel::Motor* D2RobotModel::initMotor(int radius, int speed, long unsigned int degrees, int port)
{
	Motor *motor = new Motor();
	motor->radius = radius;
	motor->speed = speed;
	motor->degrees = degrees;
	if (degrees == 0) {
			motor->activeTime = QPair<ATime, qreal>(DoInf , 0);
	}
	else {
		qreal activeTime = degrees / speed ;
		motor->activeTime = QPair<ATime, qreal>(Do , activeTime);
	}
	mMotors[port] = motor;
	mTurnoverMotors[port] = 0;
	return motor;
}

void D2RobotModel::setBeep(unsigned freq, unsigned time)
{
	mBeep.freq = freq;
	mBeep.time = time;
}

void D2RobotModel::setNewMotor(int speed, unsigned long degrees, const int port)
{
	mMotors[port]->speed = speed;
	mMotors[port]->degrees = degrees;
	if (degrees == 0) {
		mMotors[port]->activeTime = QPair<ATime, qreal>(DoInf , 0);
	}
	else {
		qreal activeTime = degrees * 1.0 / 1.0 * speed ;
		mMotors[port]->activeTime = QPair<ATime, qreal>(Do , activeTime);
	}
	mTurnoverMotors[port] = 0;
}

void D2RobotModel::countOneMotorTime(D2RobotModel::Motor &motor)
{
	if (motor.activeTime.first == Do) {
		motor.activeTime.second -= timeInterval;
		if (motor.activeTime.second <= 0) {
			motor.activeTime.first = End;
			motor.speed = 0;
		}
	}
}

void D2RobotModel::countMotorTime()
{
	countOneMotorTime(*mMotorA);
	countOneMotorTime(*mMotorB);
	countOneMotorTime(*mMotorC);
}

void D2RobotModel::countMotorTurnover()
{
	foreach (Motor *motor, mMotors) {
		int port = mMotors.key(motor);
		qreal degrees = timeInterval * 1.0 * motor->speed ;
		mTurnoverMotors[port] += degrees;
	}
}

int D2RobotModel::readEncoder(int/*inputPort::InputPortEnum*/ const port) const
{
	return mTurnoverMotors[port] / 360;//делим кол-во градусов на полный оборот
}

void D2RobotModel::resetEncoder(int/*inputPort::InputPortEnum*/ const port)
{
	mTurnoverMotors[port] = 0;
}

SensorsConfiguration &D2RobotModel::configuration()
{
	return mSensorsConfiguration;
}

D2ModelWidget *D2RobotModel::createModelWidget()
{
	mD2ModelWidget = new D2ModelWidget(this, &mWorldModel);
	return mD2ModelWidget;
}

QPair<QPoint, qreal> D2RobotModel::countPositionAndDirecnion(QPointF localPosition, qreal localDirection) const
{
	QPointF point = localPosition - rotatePoint;
	QPointF localPoint = QTransform().translate(-point.x(), -point.y()).rotate(mAngle)
						.translate(point.x(), point.y()).rotate(-mAngle)
						.map(localPosition);

	QPoint resPoint = localPoint.toPoint() + mPos.toPoint();
	qreal direction = localDirection + mAngle;
	return QPair<QPoint, qreal>(resPoint, direction);
}

QPair<QPoint, qreal> D2RobotModel::countPositionAndDirecnion(inputPort::InputPortEnum const port) const
{
	return countPositionAndDirecnion(mSensorsConfiguration.position(port), mSensorsConfiguration.direction(port));
}

bool D2RobotModel::readTouchSensor(inputPort::InputPortEnum const port)
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirecnion(port);
	bool res = mWorldModel.touchSensorReading(neededPosDir.first, neededPosDir.second, port);
	// TODO: Add checks of sensor type.
	return res;
}

int D2RobotModel::readSonarSensor(inputPort::InputPortEnum const port) const
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirecnion(port);
	return mWorldModel.sonarReading(neededPosDir.first, neededPosDir.second);
}

int D2RobotModel::readColorSensor(inputPort::InputPortEnum const port) const
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirecnion(port);
	return mWorldModel.colorSensorReading(neededPosDir.first, mSensorsConfiguration.type(port));
}

void D2RobotModel::startInit()
{
	initPosition();
	mTimer->start(timeInterval);
}

void D2RobotModel::stopRobot()
{
	mTimer->stop();
}

void D2RobotModel::countBeep()
{
	if (mBeep.time > 0) {
		mD2ModelWidget->drawBeep(QColor(Qt::red));
		mBeep.time -= timeInterval;
	} else
		mD2ModelWidget->drawBeep(QColor(Qt::green));
}

void D2RobotModel::countNewCoord()
{
	qreal vSpeed = mMotorA->speed * 2 * M_PI * mMotorA->radius * 1.0 / 120000;
	qreal uSpeed = mMotorB->speed * 2 * M_PI * mMotorB->radius * 1.0 / 120000;
	qreal gamma = 0;
	qreal deltaY = 0;
	qreal deltaX = 0;
	qreal averangeSpeed = (vSpeed + uSpeed) / 2;

	qreal const oldAngle = mAngle;
	QPointF const oldPosition = mPos;

	if (vSpeed != uSpeed) {
		qreal vRadius = vSpeed * robotHeight / (vSpeed - uSpeed);
		qreal averangeRadius = vRadius - robotHeight / 2;
		qreal angularSpeed = 0;
		if (vSpeed == -uSpeed)
			angularSpeed = vSpeed / vRadius;
		else
			angularSpeed = averangeSpeed / averangeRadius;
		gamma = timeInterval * angularSpeed;//РЅСѓР¶РµРЅ СѓРіРѕР» РІ СЂР°РґРёР°РЅР°С…
		qreal const gammaDegrees = gamma * 180 / M_PI;

		QTransform map;
		map.rotate(mAngle);
		map.translate(robotWidth / 2, averangeRadius);
		map.rotate(gammaDegrees);
		map.translate(-robotWidth / 2, -vRadius);

		QPointF newStart = map.map(QPointF(0, 0));
		deltaX = newStart.x();
		deltaY = newStart.y();

		mAngle += gammaDegrees;//РЅСѓР¶РµРЅ СѓРіРѕР» РІ РіСЂР°РґСѓСЃР°С…
	} else {
		deltaY = averangeSpeed * timeInterval * sin(mAngle * M_PI / 180);
		deltaX = averangeSpeed * timeInterval * cos(mAngle * M_PI / 180);
	}
	mPos.setX(mPos.x() + deltaX);
	mPos.setY(mPos.y() + deltaY);
	if(mAngle > 360)
		mAngle -= 360;
	QPolygonF const boundingRegion = mD2ModelWidget->robotBoundingPolygon(mPos, mAngle);
	if (mWorldModel.checkCollision(boundingRegion)) {
		mPos = oldPosition;
		mAngle = oldAngle;
	}
}

void D2RobotModel::nextFragment()
{
	// do nothing until robot gets back on the ground
	if (!mD2ModelWidget->isRobotOnTheGround())
		return;

	mPos = mD2ModelWidget->robotPos();
	countNewCoord();
	mRotatePoint = rotatePoint;
	mD2ModelWidget->draw(mPos, mAngle, mRotatePoint);
	countBeep();
	countMotorTime();
	countMotorTurnover();
}

void D2RobotModel::showModelWidget()
{
	mD2ModelWidget->init(true);
}

void D2RobotModel::rotateOn(double angle)
{
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
	mRotatePoint = rotatePoint;
	if(angle > 360)
		angle -= 360;
	mAngle += angle;
	mD2ModelWidget->draw(mPos, mAngle, mRotatePoint);
}

double D2RobotModel::rotateAngle() const
{
	return mAngle;
}
