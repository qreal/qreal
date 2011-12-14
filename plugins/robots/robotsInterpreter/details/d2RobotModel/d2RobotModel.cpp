#include "d2RobotModel.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace d2Model;

const unsigned long black   = 0xFF000000;
const unsigned long white   = 0xFFFFFFFF;
const unsigned long red     = 0xFFFF0000;
const unsigned long green   = 0xFF008000;
const unsigned long blue    = 0xFF0000FF;
const unsigned long yellow  = 0xFFFFFF00;
const unsigned long cyan    = 0xFF00FFFF;
const unsigned long magenta = 0xFFFF00FF;

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
	return mTurnoverMotors[port] / 360;// divide the number of degrees by complete revolutions count
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

bool D2RobotModel::readTouchSensor(inputPort::InputPortEnum const port)
{
	QPair<QPoint, qreal> neededPosDir = countPositionAndDirection(port);
	bool res = mWorldModel.touchSensorReading(neededPosDir.first, neededPosDir.second, port);
	// TODO: Add checks of sensor type.
	return res;
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
		gamma = timeInterval * angularSpeed;//need the angle in radians
		qreal const gammaDegrees = gamma * 180 / M_PI;

		QTransform map;
		map.rotate(mAngle);
		map.translate(robotWidth / 2, averangeRadius);
		map.rotate(gammaDegrees);
		map.translate(-robotWidth / 2, -vRadius);

		QPointF newStart = map.map(QPointF(0, 0));
		deltaX = newStart.x();
		deltaY = newStart.y();

		mAngle += gammaDegrees;//need the angle in radians
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
