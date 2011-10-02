#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPolygon>
#include <QtGui/QPainterPath>
#include <QtXml/QDomDocument>

#include "../../sensorConstants.h"
#include "wallItem.h"
#include "colorFieldItem.h"

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
	bool touchSensorReading(QPoint const &position, qreal direction, inputPort::InputPortEnum const port);
	int colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const;
	QPainterPath sonarScanningRegion(QPoint const &position, qreal direction, int range = 255) const;
	bool checkCollision(QPolygonF const &robotRegion) const;
	QList<WallItem *> const &walls() const;
        QList<ColorFieldItem *> const &colorFields() const;

	void addWall(WallItem* wall);
        void addColorField(ColorFieldItem* colorField);
        void clearScene();

        QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

private:
	QList<WallItem *> mWalls;
        QList<ColorFieldItem *> mColorFields;
	QMap<inputPort::InputPortEnum, QPointF> mTouchSensorPositionOld;
	QMap<inputPort::InputPortEnum, qreal> mTouchSensorDirectionOld;

	QPainterPath buildWallPath() const;
};

}
}
}
}
}
