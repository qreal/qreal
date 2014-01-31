#pragma once

#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WorldModel;

namespace physics {

/// An interface for 2D model physical engine strategy. Calling recalculateParams() method
/// counts position shift and angle modification for the given time interval that can be get
/// with shift() and rotation() methods correspondingly.
class PhysicsEngineBase
{
public:
	explicit PhysicsEngineBase(WorldModel const &worldModel);
	virtual ~PhysicsEngineBase();

	/// Returns robot transition vector for the time interval given in the last recalculateParams() call
	QVector2D shift() const;

	/// Returns robot angle modification for the time interval given in the last recalculateParams() call
	qreal rotation() const;

	/// Counts robot`s parameters modifications for the given time interval
	virtual void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, QPointF const &rotationCenter, QVector2D const &direction
			, QPainterPath const &robotBoundingPath) = 0;

protected:
	WorldModel const &mWorldModel;
	QVector2D mPositionShift;
	qreal mRotation;
};

}
}
}
}
}
}
