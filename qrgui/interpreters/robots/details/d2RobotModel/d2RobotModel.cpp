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

	// TODO: For testing
	mSensorsConfiguration.setSensor(inputPort::port1, sensorType::touchBoolean, QPoint(52, 25), 0);
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
	qreal point2 = mSensorsConfiguration.direction(port) + mAngle;
	bool res = mWorldModel.touchSensorReading(point, point2);

//	qDebug() << "point:" << point << ",res: "<< res;
	// TODO: Add checks of sensor type.
	return res;
}

int D2RobotModel::readSonarSensor(inputPort::InputPortEnum const port) const
{
	return mWorldModel.sonarReading(mSensorsConfiguration.position(port), mSensorsConfiguration.direction(port));
}

int D2RobotModel::readColorSensor(inputPort::InputPortEnum const port) const
{
	return mWorldModel.colorSensorReading(mSensorsConfiguration.position(port), mSensorsConfiguration.type(port));
}

void D2RobotModel::startInit()
{
	init();
	mD2ModelWidget->init();
	mTimer->start(timeInterval);
}

void D2RobotModel::stopRobot()
{
	mTimer->stop();
	mD2ModelWidget->close();
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
	mD2ModelWidget->draw(mPos, mAngle, mRotatePoint);
	countNewCoord();
	countBeep();
}
