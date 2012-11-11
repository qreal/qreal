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
#include "ejectedItem.h"

const int colorSensorWidth = 10;

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
	QPainterPath sonarScanningRegion(QPoint const &position, qreal direction, int range = 255) const;
	bool checkCollision(QPolygonF const &robotRegion) const;
	QList<WallItem *> const &walls() const;
	QList<ColorFieldItem *> const &colorFields() const;
	QList<EjectedItem *> const &ejectedItems() const;

	void addWall(WallItem* wall);
	void addColorField(ColorFieldItem* colorField);
	void addEjectedItem(EjectedItem* ejectedItem);
	void clearScene();

	bool intersectsByWall(QRectF const &rect);
	bool intersectsByStopedEjectedItems(QRectF const &rect);
	bool intersectsByNotStopedEjectedItems(QRectF const &rect);
	void checkEjectedItemsIntersects(QRectF const& itemRect, QPointF const& diffPos);

	QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

private:
	QList<WallItem *> mWalls;
	QList<ColorFieldItem *> mColorFields;
	QList<EjectedItem *> mEjectedItems;
	QMap<inputPort::InputPortEnum, QPointF> mTouchSensorPositionOld;
	QMap<inputPort::InputPortEnum, qreal> mTouchSensorDirectionOld;

	QPainterPath buildWallPath() const;
	QPainterPath buildEjectedItemPath() const;
};

}
}
}
}
}
