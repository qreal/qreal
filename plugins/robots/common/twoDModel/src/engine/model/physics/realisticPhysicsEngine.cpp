/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "realisticPhysicsEngine.h"

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "twoDModel/engine/model/timeline.h"
#include "twoDModel/engine/model/robotModel.h"
#include "src/engine/items/wallItem.h"

using namespace twoDModel::model;
using namespace physics;
using namespace mathUtils;

RealisticPhysicsEngine::RealisticPhysicsEngine(const WorldModel &worldModel
		, const QList<RobotModel *> &robots
		, const Timeline &timeline)
	: PhysicsEngineBase(worldModel, robots)
{
	QObject::connect(&timeline, &Timeline::started, [=]() {
		for (RobotModel * const robot : mRobots) {
			mVelocity[robot] = QVector2D();
			mAngularVelocity[robot] = 0.0;
			mForceMoment[robot] = 0.0;
		}
	});
}

QVector2D RealisticPhysicsEngine::positionShift(RobotModel &robot) const
{
	return mPositionShift[&robot];
}

qreal RealisticPhysicsEngine::rotation(RobotModel &robot) const
{
	return mRotation[&robot];
}

void RealisticPhysicsEngine::recalculateParameters(qreal timeInterval)
{
	for (RobotModel * const robot : mRobots) {
		recalculateParameters(timeInterval, *robot);
	}
}

void RealisticPhysicsEngine::addRobot(RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);
	mTractionForce[robot] = QVector2D();
	mReactionForce[robot] = QVector2D();
	mWallsFrictionForce[robot] = QVector2D();
	mGettingOutVector[robot] = QVector2D();
	mForceMomentDecrement[robot] = 0.0;
	mForceMoment[robot] = 0.0;
	mAngularVelocity[robot] = 0.0;
	mVelocity[robot] = QVector2D();
}

void RealisticPhysicsEngine::removeRobot(RobotModel * const robot)
{
	PhysicsEngineBase::removeRobot(robot);
}

void RealisticPhysicsEngine::recalculateParameters(qreal timeInterval, RobotModel &robot)
{
	const qreal speed1 = wheelLinearSpeed(robot, robot.leftWheel());
	const qreal speed2 = wheelLinearSpeed(robot, robot.rightWheel());
	const QVector2D direction = Geometry::directionVector(robot.rotation());

	mReactionForce[&robot] = QVector2D();
	mWallsFrictionForce[&robot] = QVector2D();
	mForceMomentDecrement[&robot] = 0;
	mGettingOutVector[&robot] = QVector2D();

	for (int i = 0; i < mWorldModel.wallsCount(); ++i) {
		findCollision(robot.robotBoundingPath(), mWorldModel.wallAt(i)->path(), robot.rotationCenter(), robot);
	}

	const bool breakMode = robot.leftWheel().breakMode || robot.rightWheel().breakMode;
	countTractionForceAndItsMoment(speed1, speed2, breakMode, direction, robot);
	recalculateVelocity(timeInterval, robot);
	applyRotationalFrictionForce(timeInterval, direction, robot);

	mPositionShift[&robot] = mGettingOutVector[&robot] + mVelocity[&robot] * timeInterval;
	mRotation[&robot] = mAngularVelocity[&robot] * timeInterval;
}

void RealisticPhysicsEngine::countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode
		, const QVector2D &direction, RobotModel &robot)
{
	if (Math::eq(speed1, 0) && Math::eq(speed2, 0)) {
		const qreal realFrictionFactor = breakMode
				? 5 // large value for practically immediate stop
				: floorFrictionCoefficient;
		mTractionForce[&robot] = -realFrictionFactor * mVelocity[&robot];
		mForceMoment[&robot] = 0;
		return;
	}

	const qreal x = robot.rotationCenter().x();
	const qreal y = robot.rotationCenter().y();
	const qreal dx = direction.x() * (robot.info().size().width() / 2);
	const qreal dy = direction.y() * (robot.info().size().height() / 2);

	const QPointF engine1Point = QPointF(x + dx + dy, y + dy - dx);
	const QPointF engine2Point = QPointF(x + dx - dy, y + dy + dx);

	const QVector2D traction1Force = direction * speed1;
	const QVector2D traction2Force = direction * speed2;
	const QVector2D friction1Force = -direction * speed1 * floorFrictionCoefficient;
	const QVector2D friction2Force = -direction * speed2 * floorFrictionCoefficient;

	const QVector2D radiusVector1 = QVector2D(engine1Point - robot.rotationCenter());
	const QVector2D radiusVector2 = QVector2D(engine2Point - robot.rotationCenter());
	const QVector2D realTractionForce1 = Geometry::projection(traction1Force, radiusVector1);
	const QVector2D realTractionForce2 = Geometry::projection(traction2Force, radiusVector2);

	// Parallelogram rule
	mTractionForce[&robot] = realTractionForce1 + realTractionForce2;
	mTractionForce[&robot] -= floorFrictionCoefficient * mVelocity[&robot];

	const qreal tractionForceMoment1 = Geometry::vectorProduct(traction1Force, radiusVector1);
	const qreal tractionForceMoment2 = Geometry::vectorProduct(traction2Force, radiusVector2);
	const qreal frictionForceMoment1 = Geometry::vectorProduct(friction1Force, radiusVector1);
	const qreal frictionForceMoment2 = Geometry::vectorProduct(friction2Force, radiusVector2);
	mForceMoment[&robot] = -tractionForceMoment1 - tractionForceMoment2 - frictionForceMoment1 - frictionForceMoment2;

	mTractionForce[&robot] += mReactionForce[&robot] + mWallsFrictionForce[&robot];
	mForceMoment[&robot] -= mForceMomentDecrement[&robot];
}

void RealisticPhysicsEngine::recalculateVelocity(qreal timeInterval, RobotModel &robot)
{
	const qreal realAngularVelocityFrictionFactor = fabs(mAngularVelocity[&robot] * angularVelocityFrictionFactor);

	mVelocity[&robot] += mTractionForce[&robot] / robot.info().mass() * timeInterval;
	mAngularVelocity[&robot] += mForceMoment[&robot] / robotInertialMoment * timeInterval;
	const qreal angularFriction = realAngularVelocityFrictionFactor / robotInertialMoment * timeInterval;
	const qreal oldAngularVelocity = mAngularVelocity[&robot];

	mAngularVelocity[&robot] -= angularFriction * Math::sign(mAngularVelocity[&robot]);

	if (oldAngularVelocity * mAngularVelocity[&robot] <= 0) {
		mAngularVelocity[&robot] = 0;
	}
}

void RealisticPhysicsEngine::applyRotationalFrictionForce(qreal timeInterval
		, const QVector2D &direction, RobotModel &robot)
{
	QVector2D rotationalFrictionForce(-direction.y(), direction.x());
	rotationalFrictionForce.normalize();

	const qreal sinus = Geometry::vectorProduct(mVelocity[&robot].normalized(), rotationalFrictionForce);
	rotationalFrictionForce *= sinus * mVelocity[&robot].length() * rotationalFrictionFactor;

	if (Geometry::scalarProduct(rotationalFrictionForce, mVelocity[&robot]) > 0) {
		rotationalFrictionForce = -rotationalFrictionForce;
	}

	const QVector2D newVelocity = mVelocity[&robot] + rotationalFrictionForce / robot.info().mass() * timeInterval;
	const qreal newProjection = Geometry::scalarProduct(newVelocity, rotationalFrictionForce);
	if (newProjection > 0) {
		const qreal oldProjection = -Geometry::scalarProduct(mVelocity[&robot], rotationalFrictionForce);
		const qreal incrementFactor = oldProjection / (oldProjection + newProjection);
		mVelocity[&robot] += rotationalFrictionForce / robot.info().mass() * timeInterval * incrementFactor;
	} else {
		mVelocity[&robot] = newVelocity;
	}
}

void RealisticPhysicsEngine::findCollision(const QPainterPath &robotBoundingRegion
		, const QPainterPath &wallBoundingRegion, const QPointF &rotationCenter, RobotModel &robot)
{
	if (!wallBoundingRegion.intersects(robotBoundingRegion)) {
		return;
	}

	const QPainterPath intersectionRegion = robotBoundingRegion.intersected(wallBoundingRegion).simplified();
	QPointF startPoint;
	QPointF endPoint;
	const qreal lengthAtom = 1;

	qreal longestProjection = 0.0;
	QPointF mostFarPointOnRobot;
	qreal longestVectorNormalSlope = 0.0;
	QVector2D sumReaction;
	int contributorsCount = 0;

	for (int i = 0; i < intersectionRegion.elementCount(); ++i) {
		const QPainterPath::Element element = intersectionRegion.elementAt(i);

		// Walking through the segments...
		if (element.isMoveTo()) {
			endPoint = QPointF(element.x, element.y);
			continue;
		}

		startPoint = endPoint;
		endPoint = QPointF(element.x, element.y);
		const QLineF currentLine(startPoint, endPoint);

		// Checking that current segment belongs to the wall path, not the robot one
		if (!Geometry::belongs(currentLine, wallBoundingRegion, lowPrecision), false) {
			continue;
		}

		const qreal currentAngle = currentLine.angle();
		const QVector2D atomicVector = QVector2D(endPoint - startPoint).normalized() * lengthAtom;

		const qreal length = Geometry::distance(startPoint, endPoint);
		const int fragmentsCount = ceil(length / lengthAtom);

		// If current line is too long then dividing it into small segments
		for (int j = 0; j <= fragmentsCount; ++j) {

			// Chosing points closer to center. In case of ideal 90 degrees angle between the wall and
			// the robot`s velocity vector resulting rotation moment must be 0
			const int transitionSign = (fragmentsCount + j) % 2 ? -1 : 1;
			const int middleIndex = fragmentsCount / 2 + transitionSign * ((j + 1) / 2);
			const QPointF currentSegmentationPoint = j == fragmentsCount
					? endPoint
					: startPoint + middleIndex * atomicVector.toPointF();

			const qreal orthogonalAngle = 90 - currentAngle;
			const QVector2D orthogonalDirectionVector = Geometry::directionVector(orthogonalAngle);

			const QLineF normalLine = Geometry::veryLongLine(currentSegmentationPoint, orthogonalDirectionVector);

			// For each point on that segments calculating reaction force vector acting from that point
			const QList<QPointF> intersectionsWithRobot = Geometry::intersection(normalLine, robotBoundingRegion
					, lowPrecision);

			QList<QPointF> intersectionsWithRobotAndWall;
			for (const QPointF &point : intersectionsWithRobot) {
				if (Geometry::belongs(point, intersectionRegion, lowPrecision)) {
					intersectionsWithRobotAndWall << point;
				}
			}

			const QPointF currentMostFarPointOnRobot =
					Geometry::closestPointTo(intersectionsWithRobotAndWall, currentSegmentationPoint);
			const QVector2D currentReactionForce = QVector2D(currentSegmentationPoint - currentMostFarPointOnRobot);
			const QVector2D currentProjection = Geometry::projection(currentReactionForce, mVelocity[&robot]);

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
	const QVector2D rawCurrentReactionForce = contributorsCount ? sumReaction / contributorsCount : QVector2D();
	const QVector2D currentReactionForce = rawCurrentReactionForce / reactionForceStabilizationCoefficient;
	const QVector2D frictionForceDirection = Geometry::directionVector(-longestVectorNormalSlope);
	const QVector2D currentFrictionForce = wallFrictionCoefficient
			 * frictionForceDirection * currentReactionForce.length();
	const QVector2D radiusVector(mostFarPointOnRobot - rotationCenter);

	mReactionForce[&robot] += currentReactionForce;
	mWallsFrictionForce[&robot] += currentFrictionForce;
	mForceMomentDecrement[&robot] += Geometry::vectorProduct(currentReactionForce, radiusVector);
	mForceMomentDecrement[&robot] += Geometry::vectorProduct(currentFrictionForce, radiusVector);
	mGettingOutVector[&robot] += rawCurrentReactionForce;
}
