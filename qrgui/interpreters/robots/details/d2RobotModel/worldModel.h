#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPolygon>

#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WorldModel
{
public:
	WorldModel();
	int sonarReading(QPoint const &position, qreal direction) const;
	bool touchSensorReading(QPoint const &position, qreal direction);
	int colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const;
	bool checkCollision(QPolygonF const &robotRegion) const;
	QList<QPair<QPointF, QPointF> > const &walls() const;

	void addWall(QPointF const &begin, QPointF const &end);
	void clearScene();

private:
	QList<QPair<QPointF, QPointF> > mWalls;
	QPointF mTouchSensorPositionOld;
};

}
}
}
}
}
