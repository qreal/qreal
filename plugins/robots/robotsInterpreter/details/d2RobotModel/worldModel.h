#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPolygon>
#include <QtGui/QPainterPath>
#include <QtXml/QDomDocument>

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
	QPainterPath sonarScanningRegion(QPoint const &position, qreal direction, int range = 255) const;
	bool checkCollision(QPolygonF const &robotRegion) const;
	QList<QPair<QPointF, QPointF> > const &walls() const;

	void addWall(QPointF const &begin, QPointF const &end);
	void clearScene();

	QDomElement serialize(QDomDocument &document) const;
	void deserialize(QDomElement const &element);

private:
	QList<QPair<QPointF, QPointF> > mWalls;
	QPointF mTouchSensorPositionOld;

	QPainterPath buildWallPath() const;
};

}
}
}
}
}
