#pragma once

#include "physicsEngineBase.h"

namespace twoDModel {
namespace model {

class Timeline;

namespace physics {

/// An implementation of 2D model physical engine with some realistic effects (like friction emulation)
class RealisticPhysicsEngine : public PhysicsEngineBase
{
public:
	explicit RealisticPhysicsEngine(const WorldModel &worldModel, const Timeline &timeline);

	void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, const QPointF &rotationCenter, qreal robotAngle
			, const QPainterPath &robotBoundingPath) override;

private:
	/// Counts and returns traction force vector taking into consideration engines speed and placement
	void countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode
			, const QPointF &rotationCenter, const QVector2D &direction);

	/// Applies all forces currently acting on robot
	void recalculateVelocity(qreal timeInterval);
	void applyRotationalFrictionForce(qreal timeInterval, const QVector2D &direction);

	/// Calculates forces and force moments acting on the robot from the walls
	void findCollision(const QPainterPath &robotBoundingRegion
			, const QPainterPath &wallBoundingRegion, const QPointF &rotationCenter);

	QVector2D mTractionForce;
	QVector2D mReactionForce;
	QVector2D mWallsFrictionForce;
	QVector2D mGettingOutVector;
	qreal mForceMomentDecrement;
	qreal mForceMoment;

	qreal mAngularVelocity;
	QVector2D mVelocity;
};

}
}
}
