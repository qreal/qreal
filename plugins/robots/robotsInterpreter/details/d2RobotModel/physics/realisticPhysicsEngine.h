#pragma once

#include "physicsEngineBase.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {
namespace physics {

/// An implementation of 2D model physical engine with some realistic effects (like friction emulation)
class RealisticPhysicsEngine : public PhysicsEngineBase
{
public:
	explicit RealisticPhysicsEngine(WorldModel const &worldModel);

	void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, QPointF const &rotationCenter, qreal robotAngle
			, QPainterPath const &robotBoundingPath) override;

private:
	/// Counts and returns traction force vector taking into consideration engines speed and placement
	void countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode
			, QPointF const &rotationCenter, QVector2D const &direction);

	/// Applies all forces currently acting on robot
	void recalculateVelocity(qreal timeInterval);
	void applyRotationalFrictionForce(qreal timeInterval, QVector2D const &direction);

	/// Calculates forces and force moments acting on the robot from the walls
	void findCollision(QPainterPath const &robotBoundingRegion
			, QPainterPath const &wallBoundingRegion, QPointF const &rotationCenter);

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
}
}
}
