#include "simplePhysicsEngine.h"

#include <QtGui/QTransform>

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "src/engine/model/constants.h"
#include "src/engine/model/worldModel.h"

using namespace twoDModel::model;
using namespace physics;
using namespace mathUtils;

SimplePhysicsEngine::SimplePhysicsEngine(WorldModel const &worldModel)
	: PhysicsEngineBase(worldModel)
{
}

void SimplePhysicsEngine::recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
		, bool engine1Break, bool engine2Break
		, QPointF const &rotationCenter, qreal robotAngle
		, QPainterPath const &robotBoundingPath)
{
	Q_UNUSED(engine1Break)
	Q_UNUSED(engine2Break)
	Q_UNUSED(rotationCenter)

	mPositionShift = QVector2D();
	mRotation = 0.0;
	if (mWorldModel.checkCollision(robotBoundingPath)) {
		return;
	}

	qreal const averageSpeed = (speed1 + speed2) / 2;

	if (!Math::eq(speed1, speed2)) {
		qreal const radius = speed1 * robotHeight / (speed1 - speed2);
		qreal const averageRadius = radius - robotHeight / 2;
		qreal angularSpeed = 0;
		qreal actualRadius = 0;
		if (Math::eq(speed1, -speed2)) {
			angularSpeed = speed1 / radius;
			actualRadius = 0;  // Radius is relative to the center of the robot.
		} else {
			angularSpeed = averageSpeed / averageRadius;
			actualRadius = averageRadius;
		}
		qreal const gammaRadians = timeInterval * angularSpeed;
		qreal const gammaDegrees = gammaRadians * 180 / pi;

		QTransform map;
		map.rotate(robotAngle);
		/// @todo robotWidth / 2 shall actually be a distance between robot center and
		// centers of the wheels by x axis.
		map.translate(-robotWidth / 2, actualRadius);
		map.rotate(gammaDegrees);
		map.translate(robotWidth / 2, -actualRadius);

		mPositionShift = QVector2D(map.map(QPointF(0, 0)));
		mRotation = gammaDegrees;
	} else {
		mPositionShift = averageSpeed * timeInterval * Geometry::directionVector(robotAngle);
	}
}
