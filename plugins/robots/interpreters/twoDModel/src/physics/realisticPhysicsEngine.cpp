#include "realisticPhysicsEngine.h"

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "src/constants.h"
#include "src/worldModel.h"
#include "src/wallItem.h"

using namespace twoDModel;
using namespace twoDModel::physics;
using namespace mathUtils;

RealisticPhysicsEngine::RealisticPhysicsEngine(WorldModel const &worldModel)
	: PhysicsEngineBase(worldModel)
	, mForceMoment(0.0)
	, mAngularVelocity(0.0)
{
}

void RealisticPhysicsEngine::recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
		, bool engine1Break, bool engine2Break
		, QPointF const &rotationCenter, qreal robotAngle
		, QPainterPath const &robotBoundingPath)
{
	QVector2D const direction = Geometry::directionVector(robotAngle);

	mReactionForce = QVector2D();
	mWallsFrictionForce = QVector2D();
	mForceMomentDecrement = 0;
	mGettingOutVector = QVector2D();

	for (int i = 0; i < mWorldModel.wallsCount(); ++i) {
		findCollision(robotBoundingPath, mWorldModel.wallAt(i)->path(), rotationCenter);
	}

	countTractionForceAndItsMoment(speed1, speed2, engine1Break || engine2Break, rotationCenter, direction);
	recalculateVelocity(timeInterval);
	applyRotationalFrictionForce(timeInterval, direction);

	mPositionShift = mGettingOutVector + mVelocity * timeInterval;
	mRotation = mAngularVelocity * timeInterval;
}

void RealisticPhysicsEngine::countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode
		, QPointF const &rotationCenter, QVector2D const &direction)
{
	if (Math::eq(speed1, 0) && Math::eq(speed2, 0)) {
		qreal const realFrictionFactor = breakMode
				? 5 // large value for practically immediate stop
				: floorFrictionCoefficient;
		mTractionForce = -realFrictionFactor * mVelocity;
		mForceMoment = 0;
		return;
	}

	qreal const x = rotationCenter.x();
	qreal const y = rotationCenter.y();
	qreal const dx = direction.x() * (robotWidth / 2);
	qreal const dy = direction.y() * (robotHeight / 2);

	QPointF const engine1Point = QPointF(x + dx + dy, y + dy - dx);
	QPointF const engine2Point = QPointF(x + dx - dy, y + dy + dx);

	QVector2D const traction1Force = direction * speed1;
	QVector2D const traction2Force = direction * speed2;
	QVector2D const friction1Force = -direction * speed1 * floorFrictionCoefficient;
	QVector2D const friction2Force = -direction * speed2 * floorFrictionCoefficient;

	QVector2D const radiusVector1 = QVector2D(engine1Point - rotationCenter);
	QVector2D const radiusVector2 = QVector2D(engine2Point - rotationCenter);
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

void RealisticPhysicsEngine::recalculateVelocity(qreal timeInterval)
{
	qreal const realAngularVelocityFrictionFactor = fabs(mAngularVelocity * angularVelocityFrictionFactor);

	mVelocity += mTractionForce / robotMass * timeInterval;
	mAngularVelocity += mForceMoment / robotInertialMoment * timeInterval;
	qreal const angularFriction = realAngularVelocityFrictionFactor / robotInertialMoment * timeInterval;
	qreal const oldAngularVelocity = mAngularVelocity;

	mAngularVelocity -= angularFriction * Math::sign(mAngularVelocity);

	if (oldAngularVelocity * mAngularVelocity <= 0) {
		mAngularVelocity = 0;
	}
}

void RealisticPhysicsEngine::applyRotationalFrictionForce(qreal timeInterval, QVector2D const &direction)
{
	QVector2D rotationalFrictionForce(-direction.y(), direction.x());
	rotationalFrictionForce.normalize();

	qreal const sinus = Geometry::vectorProduct(mVelocity.normalized(), rotationalFrictionForce);
	rotationalFrictionForce *= sinus * mVelocity.length() * rotationalFrictionFactor;

	if (Geometry::scalarProduct(rotationalFrictionForce, mVelocity) > 0) {
		rotationalFrictionForce = -rotationalFrictionForce;
	}

	QVector2D const newVelocity = mVelocity + rotationalFrictionForce / robotMass * timeInterval;
	qreal const newProjection = Geometry::scalarProduct(newVelocity, rotationalFrictionForce);
	if (newProjection > 0) {
		qreal const oldProjection = -Geometry::scalarProduct(mVelocity, rotationalFrictionForce);
		qreal const incrementFactor = oldProjection / (oldProjection + newProjection);
		mVelocity += rotationalFrictionForce / robotMass * timeInterval * incrementFactor;
	} else {
		mVelocity = newVelocity;
	}
}

void RealisticPhysicsEngine::findCollision(QPainterPath const &robotBoundingRegion
		, QPainterPath const &wallBoundingRegion, QPointF const &rotationCenter)
{
	if (!wallBoundingRegion.intersects(robotBoundingRegion)) {
		return;
	}

	QPainterPath const intersectionRegion = robotBoundingRegion.intersected(wallBoundingRegion).simplified();
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
			QList<QPointF> const intersectionsWithRobot = Geometry::intersection(normalLine, robotBoundingRegion, lowPrecision);
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
	QVector2D const radiusVector(mostFarPointOnRobot - rotationCenter);

	mReactionForce += currentReactionForce;
	mWallsFrictionForce += currentFrictionForce;
	mForceMomentDecrement += Geometry::vectorProduct(currentReactionForce, radiusVector);
	mForceMomentDecrement += Geometry::vectorProduct(currentFrictionForce, radiusVector);
	mGettingOutVector += rawCurrentReactionForce;
}
