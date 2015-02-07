#pragma once

#include "physicsEngineBase.h"

namespace twoDModel {
namespace model {
namespace physics {

/// An implementation of 2D model physical engine without any realistic effects
class SimplePhysicsEngine : public PhysicsEngineBase
{
public:
	explicit SimplePhysicsEngine(const WorldModel &worldModel);

	void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, const QPointF &rotationCenter, qreal robotAngle
			, const QPainterPath &robotBoundingPath) override;
};

}
}
}
