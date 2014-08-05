#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygon>
#include <QtXml/QDomDocument>

class QGraphicsItem;

namespace twoDModel {

namespace items {
class WallItem;
class ColorFieldItem;
}

namespace model {

class WorldModel : public QObject
{
	Q_OBJECT

public:
	WorldModel();

	int sonarReading(QPointF const &position, qreal direction) const;
	QPainterPath sonarScanningRegion(QPointF const &position, qreal direction, int range = 255) const;
	QPainterPath sonarScanningRegion(QPointF const &position, int range = 255) const;
	bool checkCollision(QPainterPath const &path) const;
	QList<items::WallItem *> const &walls() const;
	QList<items::ColorFieldItem *> const &colorFields() const;

	int wallsCount() const;
	items::WallItem *wallAt(int index) const;
	void addWall(items::WallItem *wall);
	void removeWall(items::WallItem *wall);

	void addColorField(items::ColorFieldItem *colorField);
	void removeColorField(items::ColorFieldItem *colorField);

	void clear();

	QDomElement serialize(QDomDocument &document, QPointF const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

signals:
	/// Emitted each time when model is appended with some new wall.
	void wallAdded(items::WallItem *item);

	/// Emitted each time when model is appended with some new color field item.
	void colorItemAdded(items::ColorFieldItem *item);

	/// Emitted each time when some item was removed from the 2D model world.
	void itemRemoved(QGraphicsItem *item);

private:
	bool checkSonarDistance(int const distance, QPointF const &position
			, qreal const direction, QPainterPath const &wallPath) const;

	QList<items::WallItem *> mWalls;
	QList<items::ColorFieldItem *> mColorFields;

	QPainterPath buildWallPath() const;
};

}
}
