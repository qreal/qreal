#include "d2RobotModel.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace d2Model;

unsigned long const black   = 0xFF000000;
unsigned long const white   = 0xFFFFFFFF;
unsigned long const red     = 0xFFFF0000;
unsigned long const green   = 0xFF008000;
unsigned long const blue    = 0xFF0000FF;
unsigned long const yellow  = 0xFFFFFF00;
unsigned long const cyan    = 0xFF00FFFF;
unsigned long const magenta = 0xFFFF00FF;

unsigned const touchSensorPressedSignal = 1;
unsigned const touchSensorNotPressedSignal = 0;

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
	mMotorA = initMotor(5, 0, 0, 0, false);
	mMotorB = initMotor(5, 0, 0, 1, false);
	mMotorC = initMotor(5, 0, 0, 2, false);
	setBeep(0, 0);
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
	mRotatePoint = QPointF(0, 0);  // TODO: not rotatePoint? why?
}

void D2RobotModel::clear()
{
	initPosition();
	mAngle = 0;
	mPos = QPointF(0,0);
}

D2RobotModel::Motor* D2RobotModel::initMotor(int radius, int speed, long unsigned int degrees, int port, bool isUsed)
{
	Motor *motor = new Motor();
	motor->radius = radius;
	motor->speed = speed;
	motor->degrees = degrees;
	motor->isUsed = isUsed;
	if (degrees == 0) {
		motor->activeTimeType = DoInf;
	} else {
		motor->activeTimeType = DoByLimit;
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
	mMotors[port]->isUsed = true;
	if (degrees == 0) {
		mMotors[port]->activeTimeType = DoInf;
	} else {
		mMotors[port]->activeTimeType = DoByLimit;
	}
	mTurnoverMotors[port] = 0;
}

void D2RobotModel::countMotorTurnover()
{
	foreach (Motor *motor, mMotors) {
		int port = mMotors.key(motor);
		qreal degrees = timeInterval * 1.0 * motor->speed / oneReciprocalTime;
		mTurnoverMotors[port] += degrees;
		if (motor->isUsed && (motor->activeTimeType == DoByLimit) && (mTurnoverMotors[port] >= motor->degrees)) {
			motor->speed = 0;
			motor->activeTimeType = End;
			emit d2MotorTimeout();
		}
	}
}

int D2RobotModel::readEncoder(int/*inputPort::InputPortEnum*/ const port) const
{
	return mTurnoverMotors[port];
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

QPair<QPoint, qreal> D2RobotModel::countPositionAndDirection(QPointF localPosition, qreal localDirection) const
{
	QPointF point = localPosition - rotatePoint;
	QPointF localPoint = QTransform().translate(-point.x(), -point.y()).rotate(mAngle)
			.translate(point.x(), point.y()).rotate(-mAngle)
			.map(localPosition);

	QPoint resPoint = localPoint.toPoint() + mPos.toPoint();
	qreal direction = localDirection + mAngle;
	return QPair<QPoint, qreal>(resPoint, direction);
}

QPair<QPoint, qreal> D2RobotModel::countPositionAndDirection(inputPort::InputPortEnum const port) const
{
	return countPositionAndDirection(mSensorsConfiguration.position(port), mSensorsConfiguration.direction(port));
}

int D2RobotModel::readTouchSensor(inputPort::InputPortEnum const port)
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirection(port);
	bool res = mWorldModel.touchSensorReading(neededPosDir.first, neededPosDir.second, port);
	// TODO: Add checks of sensor type.

	if (res) {
		return touchSensorPressedSignal;
	}

	return touchSensorNotPressedSignal;
}

int D2RobotModel::readSonarSensor(inputPort::InputPortEnum const port) const
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirection(port);
	return mWorldModel.sonarReading(neededPosDir.first, neededPosDir.second);
}

int D2RobotModel::readColorSensor(inputPort::InputPortEnum const port) const
{
	QImage image = printColorSensor(port);
	QHash<unsigned long, int> countsColor;

	unsigned long* data = (unsigned long*) image.bits();
	int n = image.numBytes() / 4;
	for (int i = 0; i < n; ++i) {
		unsigned long color = data[i];
		countsColor[color] ++;
	}

	switch (mSensorsConfiguration.type(port)) {
	case (sensorType::colorFull):
		return readColorFullSensor(countsColor);
	case (sensorType::colorNone):
		return readColorNoneSensor(countsColor, n);
	case (sensorType::colorRed):
		return readSingleColorSensor(red, countsColor, n);
	case (sensorType::colorGreen):
		return readSingleColorSensor(green, countsColor, n);
	case (sensorType::colorBlue):
		return readSingleColorSensor(blue, countsColor, n);
	default:
		return 0;
	}
}

QImage D2RobotModel::printColorSensor(inputPort::InputPortEnum const port) const
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirection(port);
	QPointF position = neededPosDir.first;
	qreal width = colorSensorWidth / 2.0;
	QRectF scanningRect = QRectF(position.x() -  width, position.y() - width
			, 2 * width, 2 * width);

	QImage image(scanningRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::black));
	painter.drawRect(mD2ModelWidget->scene()->itemsBoundingRect().adjusted(-width, -width, width, width));

	mD2ModelWidget->setSensorVisible(port, false);
	mD2ModelWidget->scene()->render(&painter, QRectF(), scanningRect);
	mD2ModelWidget->setSensorVisible(port, true);

	return image;
}

int D2RobotModel::readColorFullSensor(QHash<unsigned long, int> countsColor) const
{
	QList<int> values = countsColor.values();
	qSort(values);
	int maxValue = values.last();
	unsigned long maxColor = countsColor.key(maxValue);

	switch (maxColor) {
	case (black):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLACK");
		return 1;
	case (red):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "RED");
		return 5;
	case (green):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "GREEN");
		return 3;
	case (blue) :
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLUE");
		return 2;
	case (yellow):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "YELLOW");
		return 4;
	case (white):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "WHITE");
		return 6;
	case (cyan):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "CYAN");
		return 7;
	case (magenta):
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "MAGENTA");
		return 8;
	default:
		Tracer::debug(tracer::d2Model, "D2RobotModel::readColorFullSensor", "Other Color");
		return 0;
	}
}

int D2RobotModel::readSingleColorSensor(unsigned long color, QHash<unsigned long, int> countsColor, int n) const
{
	return (static_cast<double>(countsColor[color]) / static_cast<double>(n)) * 100.0;
}

int D2RobotModel::readColorNoneSensor(QHash<unsigned long, int> countsColor, int n) const
{
	double allWhite = static_cast<double>(countsColor[white]);

	QHashIterator<unsigned long, int> i(countsColor);
	while(i.hasNext()) {
		i.next();
		unsigned long color = i.key();
		if (color != white) {
			int b = (color >> 0) & 0xFF;
			int g = (color >> 8) & 0xFF;
			int r = (color >> 16) & 0xFF;
			qreal k = qSqrt(static_cast<qreal>(b * b + g * g + r * r)) / 500.0;
			allWhite += static_cast<qreal>(i.value()) * k;
		}
	}

	return (allWhite / static_cast<qreal>(n)) * 100.0;
}

int D2RobotModel::readLightSensor(inputPort::InputPortEnum const port) const
{
	Q_UNUSED(port)
	return 0;
}

void D2RobotModel::startInit()
{
	initPosition();
	mTimer->start(timeInterval);
}

void D2RobotModel::stopRobot()
{
	mMotorA->speed = 0;
	mMotorB->speed = 0;
	mMotorC->speed = 0;
}

void D2RobotModel::countBeep()
{
	if (mBeep.time > 0) {
		mD2ModelWidget->drawBeep(true);
		mBeep.time -= timeInterval;
	} else {
		mD2ModelWidget->drawBeep(false);
	}
}

void D2RobotModel::countNewCoord()
{
	Motor *motor1 = mMotorA;
	Motor *motor2 = mMotorB;

	if (mMotorC->isUsed) {
		if (!mMotorA->isUsed) {
			motor1 = mMotorC;
		} else if (!mMotorB->isUsed) {
			motor2 = mMotorC;
		}
	}

	qreal const vSpeed = motor1->speed * 2 * M_PI * motor1->radius * 1.0 / onePercentReciprocalSpeed;
	qreal const uSpeed = motor2->speed * 2 * M_PI * motor2->radius * 1.0 / onePercentReciprocalSpeed;

	qreal deltaY = 0;
	qreal deltaX = 0;
	qreal const averageSpeed = (vSpeed + uSpeed) / 2;

	qreal const oldAngle = mAngle;
	QPointF const oldPosition = mPos;

	if (vSpeed != uSpeed) {
		qreal const vRadius = vSpeed * robotHeight / (vSpeed - uSpeed);
		qreal const averageRadius = vRadius - robotHeight / 2;
		qreal angularSpeed = 0;
		qreal actualRadius = 0;
		if (vSpeed == -uSpeed) {
			angularSpeed = vSpeed / vRadius;
			actualRadius = 0;  // Radius is relative to the center of the robot.
		} else {
			angularSpeed = averageSpeed / averageRadius;
			actualRadius = averageRadius;
		}
		qreal const gammaRadians = timeInterval * angularSpeed;
		qreal const gammaDegrees = gammaRadians * 180 / M_PI;

		QTransform map;
		map.rotate(mAngle);
		// TODO: robotWidth / 2 shall actually be a distance between robot center and
		// centers of the wheels by x axis.
		map.translate(robotWidth / 2, actualRadius);
		map.rotate(gammaDegrees);
		map.translate(-robotWidth / 2, -actualRadius);

		QPointF newStart = map.map(QPointF(0, 0));
		deltaX = newStart.x();
		deltaY = newStart.y();

		mAngle += gammaDegrees;
	} else {
		deltaY = averageSpeed * timeInterval * sin(mAngle * M_PI / 180);
		deltaX = averageSpeed * timeInterval * cos(mAngle * M_PI / 180);
	}

	mPos.setX(mPos.x() + deltaX * mSpeedFactor);
	mPos.setY(mPos.y() + deltaY * mSpeedFactor);

	if(mAngle > 360) {
		mAngle -= 360;
	}

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

void D2RobotModel::setSpeedFactor(qreal speedMul)
{
	mSpeedFactor = speedMul;
}

QPointF D2RobotModel::robotPos()
{
	return this->mPos;
}
