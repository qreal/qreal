#include "d2RobotModel.h"

#include <qrkernel/settingsManager.h>
#include <utils/tracer.h>
#include "constants.h"
//#include "details/tracer.h"
#include "src/physics/realisticPhysicsEngine.h"
#include "src/physics/simplePhysicsEngine.h"

using namespace twoDModel;
using namespace twoDModel::physics;

D2RobotModel::D2RobotModel(QObject *parent)
	: QObject(parent)
	, mD2ModelWidget(nullptr)
	, mEngineA(nullptr)
	, mEngineB(nullptr)
	, mEngineC(nullptr)
//	, mDisplay(new NxtDisplay)
	, mPhysicsEngine(nullptr)
	, mTimeline(new Timeline(this))
	, mNoiseGen()
	, mNeedSync(false)
	, mPos(QPointF(0,0))
	, mAngle(0)
{
	mNoiseGen.setApproximationLevel(qReal::SettingsManager::value("approximationLevel").toUInt());
	connect(mTimeline, SIGNAL(tick()), this, SLOT(recalculateParams()), Qt::UniqueConnection);
	connect(mTimeline, SIGNAL(nextFrame()), this, SLOT(nextFragment()), Qt::UniqueConnection);
	initPosition();
}

D2RobotModel::~D2RobotModel()
{
	delete mPhysicsEngine;
}

void D2RobotModel::initPosition()
{
	if (mEngineA) {
		delete mEngineA;
	}
	if (mEngineB) {
		delete mEngineB;
	}
	if (mEngineC) {
		delete mEngineC;
	}

	mEngineA = initEngine(robotWheelDiameterInPx / 2, 0, 0, 0, false);
	mEngineB = initEngine(robotWheelDiameterInPx / 2, 0, 0, 1, false);
	mEngineC = initEngine(robotWheelDiameterInPx / 2, 0, 0, 2, false);
	setBeep(0, 0);
	mPos = mD2ModelWidget ? mD2ModelWidget->robotPos() : QPointF(0, 0);
}

void D2RobotModel::clear()
{
	initPosition();
	mAngle = 0;
	mPos = QPointF(0,0);
}

D2RobotModel::Engine *D2RobotModel::initEngine(int radius, int speed, long unsigned int degrees, int port, bool isUsed)
{
	Engine *engine = new Engine();
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

	mEngines[port] = engine;
	mTurnoverEngines[port] = 0;
	return engine;
}

void D2RobotModel::setBeep(unsigned freq, unsigned time)
{
	mBeep.freq = freq;
	mBeep.time = time;
}

void D2RobotModel::setNewMotor(int speed, uint degrees, int port, bool breakMode)
{
	// TODO: Hack for TRIK 2D model.
	if (port == 2) {
		speed = -speed;
	}

	mEngines[port]->speed = speed;
	mEngines[port]->degrees = degrees;
	mEngines[port]->isUsed = true;
	mEngines[port]->breakMode = breakMode;
	if (degrees) {
		mEngines[port]->activeTimeType = DoByLimit;
	} else {
		mEngines[port]->activeTimeType = DoInf;
	}
}

int D2RobotModel::varySpeed(int const speed) const
{
	qreal const ran = mNoiseGen.generate(mNoiseGen.approximationLevel(), varySpeedDispersion);
	return truncateToInterval(-100, 100, round(speed * (1 + ran)));
}

void D2RobotModel::countMotorTurnover()
{
	foreach (Engine * const motor, mEngines) {
		int const port = mEngines.key(motor);
		qreal const degrees = Timeline::timeInterval * motor->spoiledSpeed * onePercentAngularVelocity;
		mTurnoverEngines[port] += degrees;
		if (motor->isUsed && (motor->activeTimeType == DoByLimit) && (mTurnoverEngines[port] >= motor->degrees)) {
			motor->speed = 0;
			motor->activeTimeType = End;
			emit d2MotorTimeout();
		}
	}
}

int D2RobotModel::readEncoder(int/*inputPort::InputPortEnum*/ const port) const
{
	return mTurnoverEngines[port];
}

void D2RobotModel::resetEncoder(int/*inputPort::InputPortEnum*/ const port)
{
	mTurnoverEngines[port] = 0;
}

SensorsConfiguration &D2RobotModel::configuration()
{
	return mSensorsConfiguration;
}

D2ModelWidget *D2RobotModel::createModelWidget()
{
	mD2ModelWidget = new D2ModelWidget(this, &mWorldModel/*, mDisplay*/);
	return mD2ModelWidget;
}

//QPair<QPointF, qreal> D2RobotModel::countPositionAndDirection(robots::enums::inputPort::InputPortEnum const port) const
//{
//	QVector<SensorItem *> items = mD2ModelWidget->sensorItems();
//	SensorItem *sensor = items[port];
//	QPointF const position = sensor ? sensor->scenePos() : QPointF();
//	qreal const direction = sensor ? items[port]->rotation() + mAngle : 0;
//	return QPair<QPointF, qreal>(position, direction);
//}

//int D2RobotModel::readTouchSensor(robots::enums::inputPort::InputPortEnum const port)
//{
//	if (mSensorsConfiguration.type(port) != robots::enums::sensorType::touchBoolean
//			&& mSensorsConfiguration.type(port) != robots::enums::sensorType::touchRaw)
//	{
//		return touchSensorNotPressedSignal;
//	}
//	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
//	QPointF sensorPosition(neededPosDir.first);
//	qreal const width = sensorWidth / 2.0;
//	QRectF const scanningRect = QRectF(
//			sensorPosition.x() - width - touchSensorStrokeIncrement / 2.0
//			, sensorPosition.y() - width - touchSensorStrokeIncrement / 2.0
//			, 2 * width + touchSensorStrokeIncrement
//			, 2 * width + touchSensorStrokeIncrement);

//	QPainterPath sensorPath;
//	sensorPath.addRect(scanningRect);
//	bool const res = mWorldModel.checkCollision(sensorPath, touchSensorWallStrokeIncrement);

//	return res ? touchSensorPressedSignal : touchSensorNotPressedSignal;
//}

//int D2RobotModel::readSonarSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
//	int const res = mWorldModel.sonarReading(neededPosDir.first, neededPosDir.second);

//	return mNeedSensorNoise ? spoilSonarReading(res) : res;
//}

int D2RobotModel::spoilSonarReading(int const distance) const
{
	qreal const ran = mNoiseGen.generate(
			mNoiseGen.approximationLevel()
			, spoilSonarDispersion
			);

	return truncateToInterval(0, 255, round(distance + ran));
}

//int D2RobotModel::readColorSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	QImage const image = printColorSensor(port);
//	QHash<uint, int> countsColor;

//	uint const *data = reinterpret_cast<uint const *>(image.bits());
//	int const n = image.byteCount() / 4;
//	for (int i = 0; i < n; ++i) {
//		uint const color = mNeedSensorNoise ? spoilColor(data[i]) : data[i];
//		++countsColor[color];
//	}

//	switch (mSensorsConfiguration.type(port)) {
//	case robots::enums::sensorType::colorFull:
//		return readColorFullSensor(countsColor);
//	case robots::enums::sensorType::colorNone:
//		return readColorNoneSensor(countsColor, n);
//	case robots::enums::sensorType::colorRed:
//		return readSingleColorSensor(red, countsColor, n);
//	case robots::enums::sensorType::colorGreen:
//		return readSingleColorSensor(green, countsColor, n);
//	case robots::enums::sensorType::colorBlue:
//		return readSingleColorSensor(blue, countsColor, n);
//	default:
//		return 0;
//	}
//}

uint D2RobotModel::spoilColor(uint const color) const
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

//QImage D2RobotModel::printColorSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	if (mSensorsConfiguration.type(port) == robots::enums::sensorType::unused) {
//		return QImage();
//	}

//	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
//	QPointF const position = neededPosDir.first;
//	qreal const width = sensorWidth / 2.0;
//	QRectF const scanningRect = QRectF(position.x() - width, position.y() - width
//			, 2 * width, 2 * width);

//	QImage image(scanningRect.size().toSize(), QImage::Format_RGB32);
//	QPainter painter(&image);

//	QBrush brush(Qt::SolidPattern);
//	brush.setColor(Qt::white);
//	painter.setBrush(brush);
//	painter.setPen(QPen(Qt::white));
//	painter.drawRect(scanningRect.translated(-scanningRect.topLeft()));

//	bool const wasSelected = mD2ModelWidget->sensorItems()[port]->isSelected();
//	mD2ModelWidget->setSensorVisible(port, false);
//	mD2ModelWidget->scene()->render(&painter, QRectF(), scanningRect);
//	mD2ModelWidget->setSensorVisible(port, true);
//	mD2ModelWidget->sensorItems()[port]->setSelected(wasSelected);

//	return image;
//}

int D2RobotModel::readColorFullSensor(QHash<uint, int> const &countsColor) const
{
	if (countsColor.isEmpty()) {
		return 0;
	}

	QList<int> const values = countsColor.values();
	int maxValue = INT_MIN;
	foreach (int value, values) {
		if (value > maxValue) {
			maxValue = value;
		}
	}

	uint const maxColor = countsColor.key(maxValue);
	switch (maxColor) {
	case (black):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLACK");
		return 1;
	case (red):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "RED");
		return 5;
	case (green):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "GREEN");
		return 3;
	case (blue) :
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLUE");
		return 2;
	case (yellow):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "YELLOW");
		return 4;
	case (white):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "WHITE");
		return 6;
	case (cyan):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "CYAN");
		return 7;
	case (magenta):
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "MAGENTA");
		return 8;
	default:
//		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "Other Color");
		return 0;
	}
}

int D2RobotModel::readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const
{
	return (static_cast<double>(countsColor[color]) / static_cast<double>(n)) * 100.0;
}

int D2RobotModel::readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const
{
	double allWhite = static_cast<double>(countsColor[white]);

	QHashIterator<uint, int> i(countsColor);
	while(i.hasNext()) {
		i.next();
		uint const color = i.key();
		if (color != white) {
			int const b = (color >> 0) & 0xFF;
			int const g = (color >> 8) & 0xFF;
			int const r = (color >> 16) & 0xFF;
			qreal const k = qSqrt(static_cast<qreal>(b * b + g * g + r * r)) / 500.0;
			allWhite += static_cast<qreal>(i.value()) * k;
		}
	}

	return (allWhite / static_cast<qreal>(n)) * 100.0;
}

//int D2RobotModel::readLightSensor(robots::enums::inputPort::InputPortEnum const port) const
//{
//	// Must return 1023 on white and 0 on black normalized to percents
//	// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color

//	QImage const image = printColorSensor(port);
//	if (image.isNull()) {
//		return 0;
//	}

//	uint sum = 0;
//	uint const *data = reinterpret_cast<uint const *>(image.bits());
//	int const n = image.byteCount() / 4;

//	for (int i = 0; i < n; ++i) {
//		int const color = mNeedSensorNoise ? spoilLight(data[i]) : data[i];
//		int const b = (color >> 0) & 0xFF;
//		int const g = (color >> 8) & 0xFF;
//		int const r = (color >> 16) & 0xFF;
//		// brightness in [0..256]
//		int const brightness = 0.2126 * r + 0.7152 * g + 0.0722 * b;

//		sum += 4 * brightness; // 4 = max sensor value / max brightness value
//	}
//	qreal const rawValue = sum / n; // Average by whole region
//	return rawValue * 100 / maxLightSensorValur; // Normalizing to percents
//}

uint D2RobotModel::spoilLight(uint const color) const
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

void D2RobotModel::startInterpretation()
{
	startInit();
	mD2ModelWidget->startTimelineListening();
}

void D2RobotModel::stopRobot()
{
	mEngineA->speed = 0;
	mEngineA->breakMode = true;
	mEngineB->speed = 0;
	mEngineB->breakMode = true;
	mEngineC->speed = 0;
	mEngineC->breakMode = true;
	mD2ModelWidget->stopTimelineListening();
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

QPointF D2RobotModel::rotationCenter() const
{
	return QPointF(mPos.x() + robotWidth / 2, mPos.y() + robotHeight / 2);
}

void D2RobotModel::nextStep()
{
	mPos += mPhysicsEngine->shift().toPointF();
	mAngle += mPhysicsEngine->rotation();
}

void D2RobotModel::recalculateParams()
{
	// do nothing until robot gets back on the ground
	if (!mD2ModelWidget->isRobotOnTheGround() || !mPhysicsEngine) {
		mNeedSync = true;
		return;
	}

	synchronizePositions();

	Engine *engine1 = mEngineA;
	Engine *engine2 = mEngineB;

	if (mEngineC->isUsed) {
		if (!mEngineA->isUsed) {
			engine1 = mEngineC;
		} else if (!mEngineB->isUsed) {
			engine2 = mEngineC;
		}
	}

	engine1->spoiledSpeed = mNeedMotorNoise ? varySpeed(engine1->speed) : engine1->speed;
	engine2->spoiledSpeed = mNeedMotorNoise ? varySpeed(engine2->speed) : engine2->speed;

	qreal const speed1 = engine1->spoiledSpeed * 2 * M_PI * engine1->radius * onePercentAngularVelocity / 360;
	qreal const speed2 = engine2->spoiledSpeed * 2 * M_PI * engine2->radius * onePercentAngularVelocity / 360;

	mPhysicsEngine->recalculateParams(Timeline::timeInterval, speed1, speed2
			, engine1->breakMode, engine2->breakMode
			, rotationCenter(), mAngle
			, mD2ModelWidget->robotBoundingPolygon(mPos, mAngle));
	nextStep();
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

qreal D2RobotModel::rotateAngle() const
{
	return mAngle;
}

void D2RobotModel::setSpeedFactor(qreal speedMul)
{
	mSpeedFactor = speedMul;
	mTimeline->setSpeedFactor(speedMul);
}

void D2RobotModel::setRobotPos(QPointF const &newPos)
{
	mPos = newPos;
	mD2ModelWidget->draw(mPos, mAngle);
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
	mNeedSync = false;
	nextFragment();
}

Timeline *D2RobotModel::timeline() const
{
	return mTimeline;
}

//details::NxtDisplay *D2RobotModel::display()
//{
//	return mDisplay;
//}

void D2RobotModel::setNoiseSettings()
{
	bool const oldPhysics = mIsRealisticPhysics;
	mIsRealisticPhysics = qReal::SettingsManager::value("2DModelRealisticPhysics").toBool();
	if (oldPhysics != mIsRealisticPhysics || !mPhysicsEngine) {
		physics::PhysicsEngineBase *oldEngine = mPhysicsEngine;
		if (mIsRealisticPhysics) {
			mPhysicsEngine = new physics::RealisticPhysicsEngine(mWorldModel);
		} else {
			mPhysicsEngine = new physics::SimplePhysicsEngine(mWorldModel);
		}

		if (oldEngine) {
			delete oldEngine;
		}
	}

	mNeedSensorNoise = qReal::SettingsManager::value("enableNoiseOfSensors").toBool();
	mNeedMotorNoise = qReal::SettingsManager::value("enableNoiseOfMotors").toBool();
	mNoiseGen.setApproximationLevel(qReal::SettingsManager::value("approximationLevel").toUInt());
}

int D2RobotModel::truncateToInterval(int const a, int const b, int const res) const
{
	return (res >= a && res <= b) ? res : (res < a ? a : b);
}
