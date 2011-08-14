#include "d2RobotModel.h"
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2RobotModel::D2RobotModel(QObject *parent)
	: QObject(parent)
	, mD2ModelWidget(NULL)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(nextFragment()));
	init();
}

D2RobotModel::~D2RobotModel()
{
}

void D2RobotModel::init()
{
	mMotorA = initMotor(5, 0, 0, 0);
	mMotorB = initMotor(5, 0, 0, 1);
	mMotorC = initMotor(5, 0, 0, 2);
	setBeep(0, 0);
	mAngle = 0;
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
	mRotatePoint  = QPointF(0, 0);
}

D2RobotModel::Motor* D2RobotModel::initMotor(int radius, int speed, long unsigned int degrees, int port)
{
	Motor *motor = new Motor();
	motor->radius = radius;
	motor->speed = speed;
	motor->degrees = degrees;
	mMotors[port] = motor;
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

bool D2RobotModel::readTouchSensor(inputPort::InputPortEnum const port)
{
	QPoint point = mSensorsConfiguration.position(port) + mPos.toPoint();
	qreal direction = mSensorsConfiguration.direction(port) + mAngle;
	bool res = mWorldModel.touchSensorReading(point, direction);

	// TODO: Add checks of sensor type.
	return res;
}

int D2RobotModel::readSonarSensor(inputPort::InputPortEnum const port) const
{
	return mWorldModel.sonarReading(mSensorsConfiguration.position(port) + mPos.toPoint(), mSensorsConfiguration.direction(port) + mAngle);
}

int D2RobotModel::readColorSensor(inputPort::InputPortEnum const port) const
{
	return mWorldModel.colorSensorReading(mSensorsConfiguration.position(port) + mPos.toPoint(), mSensorsConfiguration.type(port));
}

void D2RobotModel::startInit()
{
	init();
	mD2ModelWidget->init(false);
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
		gamma = timeInterval * angularSpeed;//нужен угол в радианах
		qreal const gammaDegrees = gamma * 180 / M_PI;

		QTransform map;
		map.rotate(mAngle);
		map.translate(robotWidth, vRadius);
		map.rotate(gammaDegrees);
		map.translate(-robotWidth, -vRadius);

		QPointF newStart = map.map(QPointF(0, 0));
		deltaX = newStart.x();
		deltaY = newStart.y();

		mAngle += gammaDegrees;//нужен угол в градусах
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
	mD2ModelWidget->draw(mPos, mAngle, mRotatePoint);
	countBeep();
}

void D2RobotModel::showModelWidget()
{
	mD2ModelWidget->init(true);
}
