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

#include "simplePhysicsEngine.h"

#include <QtGui/QTransform>

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"

using namespace twoDModel::model;
using namespace physics;
using namespace mathUtils;

SimplePhysicsEngine::SimplePhysicsEngine(const WorldModel &worldModel, const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
{
}

QVector2D SimplePhysicsEngine::positionShift(RobotModel &robot) const
{
	return mPositionShift[&robot];
}

qreal SimplePhysicsEngine::rotation(RobotModel &robot) const
{
	return mRotation[&robot];
}

void SimplePhysicsEngine::recalculateParameters(qreal timeInterval)
{
	for (RobotModel * const robot : mRobots) {
		recalculateParameters(timeInterval, *robot);
	}
}

void SimplePhysicsEngine::recalculateParameters(qreal timeInterval, RobotModel &robot)
{
	if (mWorldModel.checkCollision(robot.robotBoundingPath())) {
		mPositionShift[&robot] = -mPositionShift[&robot];
		mRotation[&robot] = -mRotation[&robot];
		return;
	}

	mPositionShift[&robot] = QVector2D();
	mRotation[&robot] = 0.0;

	const qreal speed1 = wheelLinearSpeed(robot, robot.leftWheel());
	const qreal speed2 = wheelLinearSpeed(robot, robot.rightWheel());
	const qreal averageSpeed = (speed1 + speed2) / 2;

	if (!Math::eq(speed1, speed2)) {
		const qreal radius = speed1 * robot.info().size().height() / (speed1 - speed2);
		const qreal averageRadius = radius - robot.info().size().height() / 2;
		qreal angularSpeed = 0;
		qreal actualRadius = 0;
		if (Math::eq(speed1, -speed2)) {
			angularSpeed = speed1 / radius;
			actualRadius = 0;  // Radius is relative to the center of the robot.
		} else {
			angularSpeed = averageSpeed / averageRadius;
			actualRadius = averageRadius;
		}
		const qreal gammaRadians = timeInterval * angularSpeed;
		const qreal gammaDegrees = gammaRadians * 180 / pi;

		QTransform map;
		map.rotate(robot.rotation());
		/// @todo robotWidth / 2 shall actually be a distance between robot center and
		/// centers of the wheels by x axis.
		map.translate(-robot.info().size().width() / 2, actualRadius);
		map.rotate(gammaDegrees);
		map.translate(robot.info().size().width() / 2, -actualRadius);

		mPositionShift[&robot] = QVector2D(map.map(QPointF(0, 0)));
		mRotation[&robot] = gammaDegrees;
	} else {
		mPositionShift[&robot] = averageSpeed * timeInterval * Geometry::directionVector(robot.rotation());
	}
}
