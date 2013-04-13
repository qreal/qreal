#include "d2RobotModel.h"
#include "../tracer.h"
#include "../../../qrkernel/settingsManager.h"
#include "../../../../../qrutils/mathUtils/gaussNoise.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace d2Model;
using namespace mathUtils;

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

qreal const spoilColorDispersion = 2.0;
qreal const spoilLightDispersion = 1.0;
qreal const spoilSonarDispersion = 0.025;
qreal const varySpeedDispersion = 0.05;
qreal const percentSaltPepperNoise = 20.0;

D2RobotModel::D2RobotModel(QObject *parent)
		: QObject(parent)
		, mD2ModelWidget(NULL)
		, mMotorA(NULL)
		, mMotorB(NULL)
		, mMotorC(NULL)
		, mTimeline(new Timeline(this))
		, mNoiseGen()
		, mNeedSync(false)
		, mNeedSensorNoise(SettingsManager::value("enableNoiseOfSensors").toBool())
		, mNeedMotorNoise(SettingsManager::value("enableNoiseOfMotors").toBool())
{
	mAngle = 0;
	mNoiseGen.setApproximationLevel(SettingsManager::value("approximationLevel").toUInt());
	connect(mTimeline, SIGNAL(tick()), this, SLOT(recalculateParams()), Qt::UniqueConnection);
	connect(mTimeline, SIGNAL(nextFrame()), this, SLOT(nextFragment()), Qt::UniqueConnection);
	initPosition();
}

D2RobotModel::~D2RobotModel()
{
}

void D2RobotModel::initPosition()
{
	if (mMotorA) {
		delete mMotorA;
	}
	if (mMotorB) {
		delete mMotorB;
	}
	if (mMotorC) {
		delete mMotorC;
	}
	mMotorA = initMotor(5, 0, 0, 0, false);
	mMotorB = initMotor(5, 0, 0, 1, false);
	mMotorC = initMotor(5, 0, 0, 2, false);
	setBeep(0, 0);
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
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
	mMotors[port]->speed = mNeedMotorNoise ? varySpeed(speed) : speed;
	mMotors[port]->degrees = degrees;
	mMotors[port]->isUsed = true;
	if (degrees == 0) {
		mMotors[port]->activeTimeType = DoInf;
	} else {
		mMotors[port]->activeTimeType = DoByLimit;
	}
	mTurnoverMotors[port] = 0;
}

int D2RobotModel::varySpeed(const int speed) const
{
	qreal const ran = mNoiseGen.generate(
					mNoiseGen.approximationLevel()
					, varySpeedDispersion
				);

	return truncateToInterval(-100, 100, round(speed * (1 + ran)));
}

void D2RobotModel::countMotorTurnover()
{
	foreach (Motor * const motor, mMotors) {
		int const port = mMotors.key(motor);
		qreal const degrees = Timeline::timeInterval * 1.0 * motor->speed / oneReciprocalTime;
		mTurnoverMotors[port] += qAbs(degrees);
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

QPair<QPointF, qreal> D2RobotModel::countPositionAndDirection(inputPort::InputPortEnum const port) const
{
	QVector<SensorItem *> items = mD2ModelWidget->sensorItems();
	SensorItem *sensor = items[port];
	QPointF const position = sensor ? sensor->scenePos() : QPointF();
	qreal const direction = sensor ? items[port]->rotation() + mAngle : 0;
	return QPair<QPointF, qreal>(position, direction);
}

int D2RobotModel::readTouchSensor(inputPort::InputPortEnum const port)
{
	if (mSensorsConfiguration.type(port) != sensorType::touchBoolean
			&& mSensorsConfiguration.type(port) != sensorType::touchRaw)
	{
		return touchSensorNotPressedSignal;
	}
	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
	QPointF sensorPosition(neededPosDir.first);
	qreal const width = sensorWidth / 2.0;
	QRectF const scanningRect = QRectF(
			  sensorPosition.x() - width - touchSensorStrokeIncrement / 2.0
			, sensorPosition.y() - width - touchSensorStrokeIncrement / 2.0
			, 2 * width + touchSensorStrokeIncrement
			, 2 * width + touchSensorStrokeIncrement);

	QPainterPath sensorPath;
	sensorPath.addRect(scanningRect);
	bool const res = mWorldModel.checkCollision(sensorPath, touchSensorWallStrokeIncrement);

	return res ? touchSensorPressedSignal : touchSensorNotPressedSignal;
}

int D2RobotModel::readSonarSensor(inputPort::InputPortEnum const port) const
{
	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
	int const res = mWorldModel.sonarReading(neededPosDir.first, neededPosDir.second);

	return mNeedSensorNoise ? spoilSonarReading(res) : res;
}

int D2RobotModel::spoilSonarReading(int const distance) const
{
	qreal const ran = mNoiseGen.generate(
					mNoiseGen.approximationLevel()
					, spoilSonarDispersion
				);

	return truncateToInterval(0, 255, round(distance * (1 + ran)));
}

int D2RobotModel::readColorSensor(inputPort::InputPortEnum const port) const
{
	QImage const image = printColorSensor(port);
	QHash<unsigned long, int> countsColor;

	unsigned long *data = (unsigned long *) image.bits();
	int const n = image.byteCount() / 4;
	for (int i = 0; i < n; ++i) {
		unsigned long color = mNeedSensorNoise ? spoilColor(data[i]) : data[i];
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

unsigned long D2RobotModel::spoilColor(unsigned long const color) const
{
	qreal const ran = mNoiseGen.generate(
					mNoiseGen.approximationLevel()
					, spoilColorDispersion
				);
	int r = round(((color >> 16) & 0xFF) + ran);
	int g = round(((color >> 8) & 0xFF) + ran);
	int b = round(((color >> 0) & 0xFF) + ran);
	int const a = (color >> 24) & 0xFF;

	r = truncateToInterval(0, 255, r);
	g = truncateToInterval(0, 255, g);
	b = truncateToInterval(0, 255, b);

	return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF) + ((a & 0xFF) << 24);
}

QImage D2RobotModel::printColorSensor(inputPort::InputPortEnum const port) const
{
	if (mSensorsConfiguration.type(port) == sensorType::unused) {
		return QImage();
	}
	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	QPointF const position = neededPosDir.first;
	qreal const width = sensorWidth / 2.0;
	QRectF const scanningRect = QRectF(position.x() -  width, position.y() - width
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

int D2RobotModel::readSingleColorSensor(unsigned long color, QHash<unsigned long, int> const &countsColor, int n) const
{
	return (static_cast<double>(countsColor[color]) / static_cast<double>(n)) * 100.0;
}

int D2RobotModel::readColorNoneSensor(QHash<unsigned long, int> const &countsColor, int n) const
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
	// Must return 1023 on white and 0 on black normalized to percents
	// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color

	QImage const image = printColorSensor(port);

	unsigned long sum = 0;
	unsigned long *data = (unsigned long *) image.bits();
	int const n = image.byteCount() / 4;

	for (int i = 0; i < n; ++i) {
		int const color = mNeedSensorNoise ? spoilLight(data[i]) : data[i];
		int const b = (color >> 0) & 0xFF;
		int const g = (color >> 8) & 0xFF;
		int const r = (color >> 16) & 0xFF;
		// brightness in [0..256]
		int const brightness = 0.2126 * r + 0.7152 * g + 0.0722 * b;

		sum += 4 * brightness; // 4 = max sensor value / max brightness value
	}
	qreal const rawValue = sum / n; // Average by whole region
	return rawValue * 100 / maxLightSensorValur; // Normalizing to percents
}

unsigned long D2RobotModel::spoilLight(unsigned long const color) const
{
	qreal const ran = mNoiseGen.generate(
					mNoiseGen.approximationLevel()
					, spoilLightDispersion
				);
	if (ran > (1.0 - percentSaltPepperNoise / 100.0)) {
		return white;
	} else if (ran < (-1.0 + percentSaltPepperNoise / 100.0)) {
		return black;
	}

	return color;
}


void D2RobotModel::startInit()
{
	initPosition();
	mTimeline->start();
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
		mBeep.time -= Timeline::frameLength;
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

	qreal const vSpeed = motor1->speed * 2 * M_PI * motor1->radius * 1.0 / onePercentReciprocalSpeed * multiplicator;
	qreal const uSpeed = motor2->speed * 2 * M_PI * motor2->radius * 1.0 / onePercentReciprocalSpeed * multiplicator;

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
		qreal const gammaRadians = Timeline::timeInterval * angularSpeed;
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
		deltaY = averageSpeed * Timeline::timeInterval * sin(mAngle * M_PI / 180);
		deltaX = averageSpeed * Timeline::timeInterval * cos(mAngle * M_PI / 180);
	}

	QPointF const delta(deltaX, deltaY);
	mPos += delta;

	if(mAngle > 360) {
		mAngle -= 360;
	}

	QPainterPath const boundingRegion = mD2ModelWidget->robotBoundingPolygon(mPos, mAngle);
	if (mWorldModel.checkCollision(boundingRegion)) {
		mPos = oldPosition;
		mAngle = oldAngle;
	}
}

void D2RobotModel::recalculateParams()
{
	// do nothing until robot gets back on the ground
	if (!mD2ModelWidget->isRobotOnTheGround()) {
		mNeedSync = true;
		return;
	}
	synchronizePositions();
	countNewCoord();
	countMotorTurnover();
}

void D2RobotModel::nextFragment()
{
	if (!mD2ModelWidget->isRobotOnTheGround()) {
		return;
	}
	synchronizePositions();
	countBeep();
	mD2ModelWidget->draw(mPos, mAngle);
	mNeedSync = true;
}

void D2RobotModel::synchronizePositions()
{
	if (mNeedSync) {
		mPos = mD2ModelWidget->robotPos();
		mNeedSync = false;
	}
}

void D2RobotModel::showModelWidget()
{
	mD2ModelWidget->init(true);
}

void D2RobotModel::setRotation(qreal angle)
{
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
	mAngle = fmod(angle, 360);
	mD2ModelWidget->draw(mPos, mAngle);
}

double D2RobotModel::rotateAngle() const
{
	return mAngle;
}

void D2RobotModel::setSpeedFactor(qreal speedMul)
{
	mSpeedFactor = speedMul;
	mTimeline->setSpeedFactor(speedMul);
}

QPointF D2RobotModel::robotPos()
{
	return mPos;
}

void D2RobotModel::serialize(QDomDocument &target)
{
	QDomElement robot = target.createElement("robot");
	robot.setAttribute("position", QString::number(mPos.x()) + ":" + QString::number(mPos.y()));
	robot.setAttribute("direction", mAngle);
	configuration().serialize(robot, target);
	target.firstChildElement("root").appendChild(robot);
}

void D2RobotModel::deserialize(QDomElement const &robotElement)
{
	QString const positionStr = robotElement.attribute("position", "0:0");
	QStringList const splittedStr = positionStr.split(":");
	qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
	qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
	mPos = QPointF(x, y);

	mAngle = robotElement.attribute("direction", "0").toDouble();

	configuration().deserialize(robotElement);

	nextFragment();
}

Timeline *D2RobotModel::timeline() const
{
	return mTimeline;
}

void D2RobotModel::setNoiseSettings()
{
	mNeedSensorNoise = SettingsManager::value("enableNoiseOfSensors").toBool();
	mNeedMotorNoise = SettingsManager::value("enableNoiseOfMotors").toBool();
	mNoiseGen.setApproximationLevel(SettingsManager::value("approximationLevel").toUInt());
}

int D2RobotModel::truncateToInterval(int const a, int const b, int const res) const
{
	return (res >= a && res <= b) ? res : (res < a ? a : b);
}
