#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygon>
#include <QtXml/QDomDocument>

namespace twoDModel {

namespace items {
class WallItem;
class ColorFieldItem;
}

namespace model {

class WorldModel
{
public:
	WorldModel();
	int sonarReading(QPointF const &position, qreal direction) const;
	QPainterPath sonarScanningRegion(QPointF const &position, qreal direction, int range = 255) const;
	QPainterPath sonarScanningRegion(QPointF const &position, int range = 255) const;
	bool checkCollision(QPainterPath const &robotPath, int stroke = 3) const;
	QList<items::WallItem *> const &walls() const;
	QList<items::ColorFieldItem *> const &colorFields() const;

	int wallsCount() const;
	items::WallItem *wallAt(int index) const;
	void addWall(items::WallItem *wall);
	void removeWall(items::WallItem *wall);

	void addColorField(items::ColorFieldItem *colorField);
	void removeColorField(items::ColorFieldItem *colorField);

	void clearScene();

	QDomElement serialize(QDomDocument &document, QPointF const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

private:
	bool checkSonarDistance(int const distance, QPointF const &position
			, qreal const direction, QPainterPath const &wallPath) const;

	QList<items::WallItem *> mWalls;
	QList<items::ColorFieldItem *> mColorFields;

	QPainterPath buildWallPath() const;
};

}
}
