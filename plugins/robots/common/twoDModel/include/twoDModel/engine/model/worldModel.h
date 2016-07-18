/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygon>
#include <QtWidgets/QGraphicsLineItem>
#include <QtXml/QDomDocument>

#include "twoDModel/twoDModelDeclSpec.h"

class QGraphicsItem;

namespace twoDModel {

namespace items {
class WallItem;
class ColorFieldItem;
class RegionItem;
}

namespace model {

class TWO_D_MODEL_EXPORT WorldModel : public QObject
{
	Q_OBJECT

public:
	WorldModel();

	/// Measures the distance between robot and wall
	int sonarReading(const QPointF &position, qreal direction) const;

	/// Returns area which is seen by sonar sensor.
	QPainterPath sonarScanningRegion(const QPointF &position, qreal direction, int range = 255) const;

	/// Returns area which is seen by sonar sensor with zero rotation.
	QPainterPath sonarScanningRegion(const QPointF &position, int range = 255) const;

	/// Checks if the given path intersects some wall.
	bool checkCollision(const QPainterPath &path) const;

	/// Returns a set of walls in the world model. Result is mapping of wall ids to walls themselves.
	const QMap<QString, items::WallItem *> &walls() const;

	/// Returns a set of color field items in the world model. Result is mapping of field ids to fields themselves.
	const QMap<QString, items::ColorFieldItem *> &colorFields() const;

	/// Returns a set of region items in the world model. Result is mapping of field ids to regions themselves.
	const QMap<QString, items::RegionItem *> &regions() const;

	/// Returns a list of trace items on the floor.
	const QList<QGraphicsLineItem *> &trace() const;

	/// Appends \a wall into world model.
	void addWall(items::WallItem *wall);

	/// Removes \a wall from the world model.
	void removeWall(items::WallItem *wall);

	/// Appends colored item \a colorField into the world model.
	void addColorField(items::ColorFieldItem *colorField);

	/// Removes colored item \a colorField form the world model.
	void removeColorField(items::ColorFieldItem *colorField);

	/// Removes all walls, colored items, regions and robot traces from the world model.
	void clear();

	/// Appends one more segment of the given to the robot`s trace.
	void appendRobotTrace(const QPen &pen, const QPointF &begin, const QPointF &end);

	/// Removes all the segments from the current robot`s trace.
	void clearRobotTrace();

	/// Saves world to XML.
	QDomElement serialize(QDomElement &parent) const;

	/// Saves all information about the item with \a id into XML element. Item then can be recreated from
	/// this specification using createElement(QDomElement) method.
	QDomElement serializeItem(const QString &id) const;

	/// Restores world model XML specification.
	/// @param element Root element of the world model XML specification.
	void deserialize(const QDomElement &element);

	/// Searches on the scene item with the given id. Returns nullptr if not found.
	QGraphicsObject *findId(const QString &id) const;

	/// Creates element from serialized XML specification.
	void createElement(const QDomElement &element);

	/// Creates wall item described by \a element in the world model.
	void createWall(const QDomElement &element);

	/// Creates line colored item described by \a element in the world model.
	void createLine(const QDomElement &element);

	/// Creates rectangular colored item described by \a element in the world model.
	void createRectangle(const QDomElement &element);

	/// Creates ellipse colored item described by \a element in the world model.
	void createEllipse(const QDomElement &element);

	/// Creates cubic bezier colored item described by \a element in the world model.
	void createCubicBezier(const QDomElement &element);

	/// Creates stylus colored item described by \a element in the world model.
	void createStylus(const QDomElement &element);

	/// Creates region item described by \a element in the world model.
	void createRegion(const QDomElement &element);

	/// Removes item with the \a id from the world model.
	void removeItem(const QString &id);

signals:
	/// Emitted each time when model is appended with some new wall.
	void wallAdded(items::WallItem *item);

	/// Emitted each time when model is appended with some new color field item.
	void colorItemAdded(items::ColorFieldItem *item);

	/// Emitted each time when model is appended with some new item.
	void regionItemAdded(items::RegionItem *item);

	/// Emitted each time when model is appended with some new item.
	void traceItemAdded(QGraphicsLineItem *item);

	/// Emitted each time when some item was removed from the 2D model world.
	void itemRemoved(QGraphicsItem *item);

	/// Emitted when robot trace is non-empty any more or was cleared from the floor.
	void robotTraceAppearedOrDisappeared(bool appeared);

private:
	/// Returns true if ray intersects some wall.
	bool checkSonarDistance(const int distance, const QPointF &position
			, const qreal direction, const QPainterPath &wallPath) const;
	QPainterPath buildWallPath() const;

	QMap<QString, items::WallItem *> mWalls;
	QMap<QString, items::ColorFieldItem *> mColorFields;
	QMap<QString, items::RegionItem *> mRegions;
	QList<QGraphicsLineItem *> mRobotTrace;
	QScopedPointer<QDomDocument> mXmlFactory;
};

}
}

Q_DECLARE_METATYPE(QGraphicsLineItem *)
