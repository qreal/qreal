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

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"

using namespace twoDModel::model;
using namespace physics;
using namespace mathUtils;

SimplePhysicsEngine::SimplePhysicsEngine(const WorldModel &worldModel)
	: PhysicsEngineBase(worldModel)
{
}

void SimplePhysicsEngine::recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
		, bool engine1Break, bool engine2Break
		, const QPointF &rotationCenter, qreal robotAngle
		, const QPainterPath &robotBoundingPath)
{
	Q_UNUSED(engine1Break)
	Q_UNUSED(engine2Break)
	Q_UNUSED(rotationCenter)

	if (mWorldModel.checkCollision(robotBoundingPath)) {
		mPositionShift = -mPositionShift;
		mRotation = -mRotation;
		return;
	}

	mPositionShift = QVector2D();
	mRotation = 0.0;

	const qreal averageSpeed = (speed1 + speed2) / 2;

	if (!Math::eq(speed1, speed2)) {
		const qreal radius = speed1 * robotHeight / (speed1 - speed2);
		const qreal averageRadius = radius - robotHeight / 2;
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
		map.rotate(robotAngle);
		/// @todo robotWidth / 2 shall actually be a distance between robot center and
		/// centers of the wheels by x axis.
		map.translate(-robotWidth / 2, actualRadius);
		map.rotate(gammaDegrees);
		map.translate(robotWidth / 2, -actualRadius);

		mPositionShift = QVector2D(map.map(QPointF(0, 0)));
		mRotation = gammaDegrees;
	} else {
		mPositionShift = averageSpeed * timeInterval * Geometry::directionVector(robotAngle);
	}
}
