#include "d2RobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>
#include <qrutils/mathUtils/gaussNoise.h>
#include <utils/tracer.h>
#include "constants.h"

using namespace twoDModel;
using namespace qReal;
using namespace utils;

D2RobotModel::D2RobotModel(QObject *parent)
	: QObject(parent)
	, mD2ModelWidget(nullptr)
	, mEngineA(nullptr)
	, mEngineB(nullptr)
	, mEngineC(nullptr)
//	, mDisplay(new NxtDisplay)
	, mTimeline(new Timeline(this))
	, mNoiseGen()
	, mNeedSync(false)
	, mNeedSensorNoise(SettingsManager::value("enableNoiseOfSensors").toBool())
	, mNeedMotorNoise(SettingsManager::value("enableNoiseOfMotors").toBool())
	, mPos(QPointF(0,0))
	, mAngle(0)
	, mForceMoment(0)
	, mAngularVelocity(0)
{
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
	engine->motorFactor = 0;
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

	if (speed) {
		mEngines[port]->motorFactor = 0.5;
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
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLACK");
		return 1;
	case (red):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "RED");
		return 5;
	case (green):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "GREEN");
		return 3;
	case (blue) :
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "BLUE");
		return 2;
	case (yellow):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "YELLOW");
		return 4;
	case (white):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "WHITE");
		return 6;
	case (cyan):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "CYAN");
		return 7;
	case (magenta):
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "MAGENTA");
		return 8;
	default:
		Tracer::debug(Tracer::d2Model, "D2RobotModel::readColorFullSensor", "Other Color");
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

void D2RobotModel::findCollision(WallItem const &wall)
{
	QPainterPath const robotsBoundingRegion = mD2ModelWidget->robotBoundingPolygon(mPos, mAngle);
	QPainterPath const wallBoundingRegion = wall.path();
	if (!wallBoundingRegion.intersects(robotsBoundingRegion)) {
		return;
	}

	QPainterPath const intersectionRegion = robotsBoundingRegion.intersected(wall.path()).simplified();
	QPointF startPoint;
	QPointF endPoint;
	qreal const lengthAtom = 1;

	qreal longestProjection = 0.0;
	QPointF mostFarPointOnRobot;
	qreal longestVectorNormalSlope = 0.0;
	QVector2D sumReaction;
	int contributorsCount = 0;

	for (int i = 0; i < intersectionRegion.elementCount(); ++i) {
		QPainterPath::Element const element = intersectionRegion.elementAt(i);

		// Walking through the segments...
		if (element.isMoveTo()) {
			endPoint = QPointF(element.x, element.y);
			continue;
		}

		startPoint = endPoint;
		endPoint = QPointF(element.x, element.y);
		QLineF const currentLine(startPoint, endPoint);

		// Checking that current segment belongs to the wall path, not the robot one
		if (!Geometry::belongs(currentLine, wallBoundingRegion, lowPrecision), false) {
			continue;
		}

		qreal const currentAngle = currentLine.angle();
		QVector2D const atomicVector = QVector2D(endPoint - startPoint).normalized() * lengthAtom;

		qreal const length = Geometry::distance(startPoint, endPoint);
		int const fragmentsCount = ceil(length / lengthAtom);

		// If current line is too long then dividing it into small segments
		for (int j = 0; j <= fragmentsCount; ++j) {

			// Chosing points closer to center. In case of ideal 90 degrees angle between the wall and
			// the robot`s velocity vector resulting rotation moment must be 0
			int const transitionSign = (fragmentsCount + j) % 2 ? -1 : 1;
			int const middleIndex = fragmentsCount / 2 + transitionSign * ((j + 1) / 2);
			QPointF const currentSegmentationPoint = j == fragmentsCount
					? endPoint
					: startPoint + middleIndex * atomicVector.toPointF();

			qreal const orthogonalAngle = 90 - currentAngle;
			QVector2D const orthogonalDirectionVector = Geometry::directionVector(orthogonalAngle);

			QLineF const normalLine = Geometry::veryLongLine(currentSegmentationPoint, orthogonalDirectionVector);

			// For each point on that segments calculating reaction force vector acting from that point
			QList<QPointF> const intersectionsWithRobot = Geometry::intersection(normalLine, robotsBoundingRegion, lowPrecision);
			QList<QPointF> intersectionsWithRobotAndWall;
			foreach (QPointF const &point, intersectionsWithRobot) {
				if (Geometry::belongs(point, intersectionRegion, lowPrecision)) {
					intersectionsWithRobotAndWall << point;
				}
			}

			QPointF const currentMostFarPointOnRobot =
					Geometry::closestPointTo(intersectionsWithRobotAndWall, currentSegmentationPoint);
			QVector2D const currentReactionForce = QVector2D(currentSegmentationPoint - currentMostFarPointOnRobot);
			QVector2D const currentProjection = Geometry::projection(currentReactionForce, mVelocity);

			sumReaction += currentReactionForce;
			++contributorsCount;

			// The result reaction force is maximal vector from obtained ones
			if (!currentMostFarPointOnRobot.isNull() && currentProjection.length() > longestProjection) {
				longestProjection = currentProjection.length();
				mostFarPointOnRobot = currentMostFarPointOnRobot;
				longestVectorNormalSlope = currentAngle;
			}
		}
	}

	// Reaction force is an average between all reaction forces from small wall parts
	QVector2D const rawCurrentReactionForce = contributorsCount ? sumReaction / contributorsCount : QVector2D();
	QVector2D const currentReactionForce = rawCurrentReactionForce / reactionForceStabilizationCoefficient;
	QVector2D const frictionForceDirection = Geometry::directionVector(-longestVectorNormalSlope);
	QVector2D const currentFrictionForce = wallFrictionCoefficient
			 * frictionForceDirection * currentReactionForce.length();
	QVector2D const radiusVector(mostFarPointOnRobot - rotationCenter());

	mReactionForce += currentReactionForce;
	mWallsFrictionForce += currentFrictionForce;
	mForceMomentDecrement += Geometry::vectorProduct(currentReactionForce, radiusVector);
	mForceMomentDecrement += Geometry::vectorProduct(currentFrictionForce, radiusVector);
	mGettingOutVector += rawCurrentReactionForce;
}

void D2RobotModel::countNewForces()
{
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

	qreal const speed1 = engine1->spoiledSpeed * 2 * M_PI * engine1->radius * onePercentAngularVelocity / 360 * engine1->motorFactor;
	qreal const speed2 = engine2->spoiledSpeed * 2 * M_PI * engine2->radius * onePercentAngularVelocity / 360 * engine2->motorFactor;

	countTractionForceAndItsMoment(speed1, speed2, engine1->breakMode || engine2->breakMode);
	recalculateVelocity();
	applyRotationalFrictionForce();
}

void D2RobotModel::countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode)
{
	if (Math::eq(speed1, 0) && Math::eq(speed2, 0)) {
		qreal const realFrictionFactor = breakMode
				? 5 // large value for practically immediate stop
				: floorFrictionCoefficient;
		mTractionForce = -realFrictionFactor * mVelocity;
		mForceMoment = 0;
		return;
	}

	QPointF const currentRotationCenter = rotationCenter();
	QVector2D const direction = robotDirectionVector();

	qreal const x = currentRotationCenter.x();
	qreal const y = currentRotationCenter.y();
	qreal const dx = direction.x() * (robotWidth / 2);
	qreal const dy = direction.y() * (robotHeight / 2);

	QPointF const engine1Point = QPointF(x + dx + dy, y + dy - dx);
	QPointF const engine2Point = QPointF(x + dx - dy, y + dy + dx);

	QVector2D const traction1Force = direction * speed1;
	QVector2D const traction2Force = direction * speed2;
	QVector2D const friction1Force = -direction * speed1 * floorFrictionCoefficient;
	QVector2D const friction2Force = -direction * speed2 * floorFrictionCoefficient;

	QVector2D const radiusVector1 = QVector2D(engine1Point - currentRotationCenter);
	QVector2D const radiusVector2 = QVector2D(engine2Point - currentRotationCenter);
	QVector2D const realTractionForce1 = Geometry::projection(traction1Force, radiusVector1);
	QVector2D const realTractionForce2 = Geometry::projection(traction2Force, radiusVector2);

	// Parallelogram rule
	mTractionForce = realTractionForce1 + realTractionForce2;
	mTractionForce -= floorFrictionCoefficient * mVelocity;

	qreal const tractionForceMoment1 = Geometry::vectorProduct(traction1Force, radiusVector1);
	qreal const tractionForceMoment2 = Geometry::vectorProduct(traction2Force, radiusVector2);
	qreal const frictionForceMoment1 = Geometry::vectorProduct(friction1Force, radiusVector1);
	qreal const frictionForceMoment2 = Geometry::vectorProduct(friction2Force, radiusVector2);
	mForceMoment = -tractionForceMoment1 - tractionForceMoment2 - frictionForceMoment1 - frictionForceMoment2;

	mTractionForce += mReactionForce + mWallsFrictionForce;
	mForceMoment -= mForceMomentDecrement;
}

void D2RobotModel::recalculateVelocity()
{
	qreal const realAngularVelocityFrictionFactor = fabs(mAngularVelocity * angularVelocityFrictionFactor);

	mVelocity += mTractionForce / robotMass * Timeline::timeInterval;
	mAngularVelocity += mForceMoment / robotInertialMoment * Timeline::timeInterval;
	qreal const angularFriction = realAngularVelocityFrictionFactor / robotInertialMoment * Timeline::timeInterval;
	qreal const oldAngularVelocity = mAngularVelocity;

	mAngularVelocity -= angularFriction * Math::sign(mAngularVelocity);

	if (oldAngularVelocity * mAngularVelocity <= 0) {
		mAngularVelocity = 0;
	}
}

void D2RobotModel::applyRotationalFrictionForce()
{
	QVector2D const direction = robotDirectionVector();
	QVector2D rotationalFrictionForce(-direction.y(), direction.x());
	rotationalFrictionForce.normalize();

	qreal const sinus = Geometry::vectorProduct(mVelocity.normalized(), rotationalFrictionForce);
	rotationalFrictionForce *= sinus * mVelocity.length() * rotationalFrictionFactor;

	if (Geometry::scalarProduct(rotationalFrictionForce, mVelocity) > 0) {
		rotationalFrictionForce = -rotationalFrictionForce;
	}

	QVector2D const newVelocity = mVelocity + rotationalFrictionForce / robotMass * Timeline::timeInterval;
	qreal const newProjection = Geometry::scalarProduct(newVelocity, rotationalFrictionForce);
	if (newProjection > 0) {
		qreal const oldProjection = -Geometry::scalarProduct(mVelocity, rotationalFrictionForce);
		qreal const incrementFactor = oldProjection / (oldProjection + newProjection);
		mVelocity += rotationalFrictionForce / robotMass * Timeline::timeInterval * incrementFactor;
	} else {
		mVelocity = newVelocity;
	}
}

QPointF D2RobotModel::rotationCenter() const
{
	return QPointF(mPos.x() + robotWidth / 2, mPos.y() + robotHeight / 2);
}

QVector2D D2RobotModel::robotDirectionVector() const
{
	return Geometry::directionVector(mAngle);
}

void D2RobotModel::nextStep()
{
	qreal const timeInterval = Timeline::timeInterval;
	mPos += mVelocity.toPointF() * timeInterval;
	mAngle += mAngularVelocity * timeInterval;
}

void D2RobotModel::recalculateParams()
{
	// do nothing until robot gets back on the ground
	if (!mD2ModelWidget->isRobotOnTheGround()) {
		mNeedSync = true;
		return;
	}

	synchronizePositions();

	mReactionForce = QVector2D();
	mWallsFrictionForce = QVector2D();
	mForceMomentDecrement = 0;
	mGettingOutVector = QVector2D();

	for (int i = 0; i < mWorldModel.wallsCount(); ++i) {
		findCollision(*(mWorldModel.wallAt(i)));
	}

	countNewForces();
	getFromWalls();
	nextStep();
	countMotorTurnover();
}

void D2RobotModel::getFromWalls()
{
	mPos += mGettingOutVector.toPointF();
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
	mNeedSensorNoise = SettingsManager::value("enableNoiseOfSensors").toBool();
	mNeedMotorNoise = SettingsManager::value("enableNoiseOfMotors").toBool();
	mNoiseGen.setApproximationLevel(SettingsManager::value("approximationLevel").toUInt());
}

int D2RobotModel::truncateToInterval(int const a, int const b, int const res) const
{
	return (res >= a && res <= b) ? res : (res < a ? a : b);
}
