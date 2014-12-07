#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygon>
#include <QtWidgets/QGraphicsLineItem>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/abstractItem.h>

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

	/// Measures the distance between robot and wall
	int sonarReading(QPointF const &position, qreal direction) const;

	/// Returns area which is seen by sonar sensor.
	QPainterPath sonarScanningRegion(QPointF const &position, qreal direction, int range = 255) const;

	/// Returns area which is seen by sonar sensor with zero rotation.
	QPainterPath sonarScanningRegion(QPointF const &position, int range = 255) const;

	/// Checks if the given path intersects some wall.
	bool checkCollision(QPainterPath const &path) const;

	/// Returns a list of walls in the world model.
	QList<items::WallItem *> const &walls() const;

	QList<items::ColorFieldItem *> const &colorFields() const;

	int wallsCount() const;
	items::WallItem *wallAt(int index) const;
	void addWall(items::WallItem *wall);
	void removeWall(items::WallItem *wall);

	void addColorField(items::ColorFieldItem *colorField);
	void removeColorField(items::ColorFieldItem *colorField);

	void clear();

	/// Appends one more segment of the given to the robot`s trace.
	void appendRobotTrace(QPen const &pen, QPointF const &begin, QPointF const &end);

	/// Removes all the segments from the current robot`s trace.
	void clearRobotTrace();

	/// Saves world to XML.
	QDomElement serialize(QDomDocument &document, QPointF const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

signals:
	/// Emitted each time when model is appended with some new wall.
	void wallAdded(items::WallItem *item);

	/// Emitted each time when model is appended with some new color field item.
	void colorItemAdded(items::ColorFieldItem *item);

	/// Emitted each time when model is appended with some new item.
	void otherItemAdded(QGraphicsItem *item);

	/// Emitted each time when some item was removed from the 2D model world.
	void itemRemoved(QGraphicsItem *item);

	/// Emitted when robot trace is non-empty any more or was cleared from the floor.
	void robotTraceAppearedOrDisappeared(bool appeared);

	void itemAdded(graphicsUtils::AbstractItem *item);

private:
	/// Returns true if ray intersects some wall.
	bool checkSonarDistance(int const distance, QPointF const &position
			, qreal const direction, QPainterPath const &wallPath) const;
	QPainterPath buildWallPath() const;

	QList<items::WallItem *> mWalls;
	QList<items::ColorFieldItem *> mColorFields;
	QList<QGraphicsLineItem *> mRobotTrace;
};

}
}
