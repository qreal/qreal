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
	int sonarReading(QPoint const &position, qreal direction) const;
	bool touchSensorReading(QPoint const &position, qreal direction) const;
	int colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const;
	bool checkCollision(QPolygon const &robotRegion) const;
	QList<QPair<QPoint, QPoint> > const &walls() const;
	void addWall(QPoint const &begin, QPoint const &end);

private:
	QList<QPair<QPoint, QPoint> > mWalls;
};

}
}
}
}
}
