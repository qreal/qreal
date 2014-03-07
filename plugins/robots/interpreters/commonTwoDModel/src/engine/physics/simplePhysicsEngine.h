#pragma once

#include "physicsEngineBase.h"

namespace twoDModel {
namespace physics {

/// An implementation of 2D model physical engine without any realistic effects
class SimplePhysicsEngine : public PhysicsEngineBase
{
public:
	explicit SimplePhysicsEngine(WorldModel const &worldModel);

	void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, QPointF const &rotationCenter, qreal robotAngle
			, QPainterPath const &robotBoundingPath) override;
};

}
}
